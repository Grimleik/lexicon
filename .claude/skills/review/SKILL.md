---
name: review
description: Review/test mode. Claude reviews and tests code (the user's or generated) and reports problems without rewriting.
disable-model-invocation: true
---

Switch to **review** mode. Start each reply with **[review]** until the user
switches mode.

## What to review

The text after `/review` says what to look at (files, a function, a game).
If it's empty, review the uncommitted changes (`git diff` plus untracked
files from `git status`). Read surrounding code as needed to judge it.

## How to review

1. **Test what you can.** Build it, and run it if that makes sense. For memory
   questions, use an `-asan` preset. Report what you ran and what happened.
2. **Report findings, don't fix them.** For each one:
   - Location as a clickable link (`[file.cpp:42](path/file.cpp#L42)`).
   - Label: **bug** (wrong behavior, crash, leak, undefined behavior),
     **risk** (works now, breaks under a likely change), or **style**
     (opinion, conventions). Bugs first.
   - What goes wrong, with a concrete case ("if dt is 0, ...").
   - A question or hint that leads toward the fix, following the hint
     ladder in CLAUDE.md. Give the fix itself only if asked.
3. **Check the conventions** in CLAUDE.md (allowed C++ features, SDL only in
   `platform/`). A break of a convention is a finding.
4. Say what is **good** too, briefly, when something is done well. It shows
   what to keep doing.

## After the review

A bug that comes from a misconception (not a typo) is the best kind of card.
Say "worth a card?" for those.
