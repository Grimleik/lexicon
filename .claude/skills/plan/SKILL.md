---
name: plan
description: Draft a game's definition of done (DoD) and questions from the roadmap, with a reason for each item, for the user to review and own.
disable-model-invocation: true
---

The user wants a definition of done for a game, or wants to sharpen an
existing one. The text after `/plan` names the game (number or name) and may
add wishes ("I want to practice shipping"). This is a one-off action: don't
change the current mode.

Why this exists: the user can't yet judge what a project should teach. You
draft; they own the result by understanding every item. Drafting is your job,
deciding is theirs.

## Gather

- The game's row in `notes/roadmap.md`: gameplay outcomes, systems outcome,
  side quest, and its "Read" sources. Also the per-game routine (time box).
- The game's devlog in `notes/` if it exists. Keep what the user already
  wrote; build on it, don't replace it.
- Earlier devlogs' retros, if any: things that were hard or left undone
  may belong here.

## Draft

**Definition of done.** Each item:
- is **checkable by running the program** (or by looking at something
  concrete, like a file in a release folder). "Read X" or "understand Y" are
  not DoD items; reading goes to the Reading list, understanding to the
  questions.
- is tagged **gameplay**, **systems** (the concept the game teaches,
  usually measured), or **habit** (shipping, profiling, testing, devlog).
- has a one-line **why**: what the user learns or proves by doing it.
- turns vague wishes into checks. "Ship it" becomes where it goes, what is
  in it, and how you check that it runs outside the dev setup.

**Scope.** Fit the roadmap's time box (small game ~1 weekend, slice ~3-4
weekends). Anything that doesn't fit goes to a **Stretch** list, not the DoD.
Fewer, sharper items beat a long list.

**Questions (3-7).** Drawn from the outcomes. Answerable in the retro with
something measured or seen, not just read. Prefer "what happens if" and
"why" over "what is".

## Present, then wait

Show the draft in chat, with the why for each item. Then ask:
- Which items can't you explain the reason for yet? (Ask about those;
  don't just delete them.)
- Anything you want that's missing, or that you'd cut?

Only after the user agrees, write it into the devlog
(`notes/NN_name.md`), keeping their own sections and wording.
