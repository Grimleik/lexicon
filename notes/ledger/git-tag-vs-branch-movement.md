---
type: recall
tags: [git]
box: 1
created: 2026-11-25
last_reviewed: 2026-11-25
source: session (shipping sdl_check-v0.1)
---

## Q

You tag a commit `v0.1`, then make two more commits on `master`. Where do
`v0.1` and `master` point now, and why?

## A

`v0.1` still points at the tagged commit; `master` points at the newest one.
Both are names for a commit, but a branch moves forward with each new commit
and a tag never moves: it's a fixed alias for that commit's hash. That's why
a tag marks a released version.

## Read more

- Pro Git, [2.6 Git Basics - Tagging](https://git-scm.com/book/en/v2/Git-Basics-Tagging):
  lightweight vs annotated tags, pushing tags.
- Pro Git, [3.1 Git Branching - Branches in a Nutshell](https://git-scm.com/book/en/v2/Git-Branching-Branches-in-a-Nutshell):
  a branch is a movable pointer; how HEAD and commits move it.

## Log

