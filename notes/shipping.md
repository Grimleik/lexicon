# Shipping checklist

From a finished branch to a live blog post. Steps 5-6 of the routine in
`roadmap.md`, with the exact commands. Example: `00_window`, version 0.1.

## 1. Before the PR (on the branch)

- [ ] DoD items in `notes/00_window.md` checked (or moved to Stretch).
- [ ] The ship target exists in `CMakeLists.txt`, under the game's
      `add_game` line: `luma_ship(00_window 0.1)`. The version here
      **must match the tag** in step 3, or the release job stops.
- [ ] Local release check: `cmake --preset windows-release`, then
      `cmake --build build/windows-release --target ship_00_window`.
      Unzip `build/windows-release/ship/00_window-v0.1.zip` somewhere
      outside the repo and double-click the exe.
- [ ] Pushed: `git push`. CI is green (Actions tab or the check mark on
      the commit).

## 2. PR and merge

- [ ] On GitHub: **Pull requests → New** → base `master`, compare
      `00_window`. Title = the game, body = a few lines on what's in it.
- [ ] Wait for green CI on the PR.
- [ ] Run `/review` locally on the branch, fix bugs, push again.
- [ ] **Merge** (a merge commit, not squash: it keeps the branch history).

## 3. Tag = release

```sh
git switch master
git pull                       # get the merge commit
git tag -a 00_window-v0.1 -m "00_window v0.1: window + moving rect"
git push origin 00_window-v0.1
```

- [ ] The release job builds the zip and creates a GitHub release named
      `00_window-v0.1` (Actions tab, then **Releases**).
- [ ] Download the zip from the release page and play it once.
- Milestones: bump the version (`v0.2`) and name the milestone in the tag
  message (`-m "m2: shadows"`).

## 4. Retro

- [ ] Fill in the Retro section of the devlog: answers to the questions,
      backed by something measured or seen. Copilot setup review included.

## 5. Blog post (on master)

First time only: **Settings → Pages → Deploy from a branch → `master`,
`/docs` → Save.** The site lives at https://grimleik.github.io/luma/.

- [ ] Post file: `docs/_posts/YYYY-MM-DD-00-window.md` (Jekyll only picks
      up posts with a date in the file name). Start it with:
      ```
      ---
      title: "00 Window: ..."
      ---
      ```
- [ ] Images/GIFs in `docs/assets/00_window/`, linked as
      `![frame times](/luma/assets/00_window/frametime.png)`.
      GIFs on Windows: [ScreenToGif](https://www.screentogif.com/).
- [ ] Code links at the tag, so they never break when the code changes:
      `https://github.com/Grimleik/luma/blob/00_window-v0.1/games/00_window/00_window.cpp#L40-L60`
      (on GitHub: open the file at the tag, click a line number, shift-click
      another, copy the URL).
- [ ] Download link:
      `https://github.com/Grimleik/luma/releases/download/00_window-v0.1/00_window-v0.1.zip`
- [ ] Commit straight to master (markdown only, CI skips it) and push.
      Pages rebuilds in a minute or two; check the post on the site.
- [ ] Update the Status column in `README.md`.
