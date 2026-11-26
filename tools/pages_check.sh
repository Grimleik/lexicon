#!/usr/bin/env bash
# pages_check.sh - can this repo host a GitHub Pages site, and is it up?
#
# Usage (from the repo root, in Git Bash):
#   tools/pages_check.sh                     read-only: plan, visibility, Pages status
#   tools/pages_check.sh --enable <branch>   turn Pages on, serving <branch>:/docs
#
# Needs the GitHub CLI, logged in:
#   winget install GitHub.cli     (then open a new terminal)
#   gh auth login
#
# How Pages works here: GitHub runs Jekyll (a static site generator) on the
# docs/ folder of one branch and serves the result at
# https://<user>.github.io/<repo>/. Markdown files become pages. No build
# step on our side. docs/index.md is the front page.
#
# Rules that decide whether it works (as of 2026):
#   - public repo:  Pages on any plan.
#   - private repo: Pages needs a paid plan (Pro/Team). The SITE is still
#     public to everyone, but links from it into the repo (code at a tag,
#     release downloads) give readers a 404.

set -u

if ! command -v gh >/dev/null 2>&1; then
    echo "gh (GitHub CLI) not found. Install: winget install GitHub.cli, then: gh auth login"
    exit 1
fi
if ! gh auth status >/dev/null 2>&1; then
    echo "gh is installed but not logged in. Run: gh auth login"
    exit 1
fi

# owner/name of the repo this folder's git remote points to.
repo=$(gh repo view --json nameWithOwner -q .nameWithOwner) || exit 1
visibility=$(gh repo view --json visibility -q .visibility)
# GET /user returns your own plan (free, pro, ...) when you're logged in.
plan=$(gh api user -q '.plan.name // "unknown"')

echo "Repo:        $repo ($visibility)"
echo "Your plan:   $plan"
if [ "$visibility" = "PUBLIC" ] || [ "$plan" != "free" ]; then
    echo "Pages:       allowed"
else
    echo "Pages:       NOT allowed (private repo on the free plan)"
fi
if [ "$visibility" != "PUBLIC" ]; then
    echo "Note:        code links and release downloads won't work for readers until the repo is public."
fi

# Optional: switch Pages on. POST creates the Pages config; it fails with
# 409 if Pages is already on (that's fine, the status below shows it).
if [ "${1:-}" = "--enable" ]; then
    branch="${2:?usage: --enable <branch>}"
    echo
    echo "Enabling Pages from $branch:/docs ..."
    gh api -X POST "repos/$repo/pages" \
        -f "source[branch]=$branch" -f "source[path]=/docs" >/dev/null \
        && echo "Enabled. The first build takes a minute or two."
fi

echo
# GET .../pages returns 404 when Pages is off.
# (-q is gh's built-in jq filter, so we don't need jq installed.)
if url=$(gh api "repos/$repo/pages" -q .html_url 2>/dev/null); then
    src=$(gh api "repos/$repo/pages" -q '"\(.source.branch):\(.source.path)"')
    build=$(gh api "repos/$repo/pages/builds/latest" -q .status 2>/dev/null || echo "none yet")
    echo "Pages is ON:  $url"
    echo "Source:       $src"
    echo "Last build:   $build   (built = done, building/queued = wait, errored = see Actions tab)"
    # Ask the live site for its HTTP status: 200 = you can view it.
    code=$(curl -s -o /dev/null -w "%{http_code}" "$url")
    echo "Site answers: HTTP $code"
else
    echo "Pages is OFF. Push a branch that has docs/index.md, then:"
    echo "  tools/pages_check.sh --enable <branch>"
fi
