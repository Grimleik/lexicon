---
name: implement
description: Implement mode. Claude writes code within a scope the user gives, then summarizes what is worth knowing.
disable-model-invocation: true
---

Switch to **implement** mode. Start each reply with **[implement]** until the
user switches mode.

## Before writing

- **Scope:** the user's request (the text after `/implement`) defines what you
  may change. If it doesn't say which files or systems, ask in one line,
  suggesting the scope you'd pick. Don't change anything outside it. If you
  find something outside the scope that needs changing, mention it.
- If the task is large or has a real design choice, give a short plan
  (a few bullets, a diagram if the structure matters) and wait for an OK.

## While writing

- Follow the project conventions in CLAUDE.md (C-style C++20, allowed
  feature list, no SDL outside `platform/`, `core/` by extraction only).
- Comment the *why*, not the *what*. The user will read this code later to
  learn from it.
- Build it (`cmake --build build/<preset>`) and run it if that's possible,
  and report what you checked. Don't say it works if you didn't check.

## After writing

End with a short **Worth knowing** section: 2-3 bullets on the ideas in this
code that the user would need to write it themselves (a technique, a gotcha,
an API behavior, why something is structured the way it is). Define terms.
If one of them is worth a card, say "worth a card?" and let the user draft it.
