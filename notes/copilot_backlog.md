# Copilot backlog

Ideas for the copilot setup (modes, ledger, quiz) that are deliberately
postponed. Phase 1 stays small. Move something here into use only when real
use shows it's missing.

## Ledger

- **Better scheduler:** SM-2 or FSRS (Anki's current algorithm) instead of
  Leitner boxes. Only worth it at ~100+ cards that I actually review.
- **Categories:** fixed tag set (c++, dsa, graphics, game, hardware, tools...)
  once free-form tags get messy.
- **Merging and pruning:** when duplicates or near-duplicates pile up.
- **Cards anchored to code:** a `ref: file:line` field so the quiz can show
  my current code. Needs a plan for when the code changes or moves.
- **Calibration stats:** from the review logs, how often "sure" is wrong,
  per tag.
- **Move the ledger out of this repo** into a personal, cross-project one
  with project tags (extract it once a second project needs it, like `core/`).
- **Track skipped quizzes** so the nudge can get firmer after repeated skips.

## Modes

- **Auto mode:** Claude picks the mode. Dropped for now: it tends to drift
  toward implementing. Revisit only with rule-based defaults (e.g. CMake =
  implement, game code = copilot).
- **Productive failure:** for some concept topics, let me attempt before any
  explanation. Needs a rule for which topics.

## Visual / interactive

- Interactive HTML explainers (Red Blob Games style) for things a static
  diagram can't show well.
- `playground/` CMake integration once the first demo exists.

## Session habits

- **End-of-session retrieval:** "3 things I learned today", written from
  memory, feeding the devlog and card drafts.
