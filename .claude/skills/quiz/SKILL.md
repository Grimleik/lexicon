---
name: quiz
description: Quiz mode. Review due spaced-repetition cards from notes/ledger and update their schedule.
disable-model-invocation: true
---

Switch to **quiz** mode. Start each reply with **[quiz]** until the quiz ends
or the user switches mode. Format and schedule: `notes/ledger/README.md`.
Read it first.

## Pick cards

- Due cards only: `last_reviewed + interval(box) <= today`. Most overdue
  first, but mix tags so two cards on the same topic aren't back to back
  (interleaving).
- Default to at most **5 cards**. The text after `/quiz` can change that
  (`/quiz 10`), or ask for a tag (`/quiz c++`).
- If nothing is due and the user wants practice anyway, quiz from the lowest
  boxes but **don't change the schedule** (early review doesn't count).

## One card at a time

1. Show only the question. For an **exercise** card, show the task. The user
   does it in the terminal or the repo.
2. Ask for confidence **before** the answer: guess / unsure / sure.
3. The user answers. Compare with the card's answer. Judge the meaning, not
   the wording. For exercises, check the result yourself (run the command,
   read the code).
4. Say right or wrong and why, in a few lines. Then show the card's answer.
   The user may overrule your verdict. If wrong and the card has a
   `## Read more` section, point to it.
5. **Sure but wrong** is the most important case: it points to a
   misconception. Spend a little time on it, and ask whether the card itself
   is unclear and should be rewritten.

## Update the card file

- Right: `box` + 1 (max 5). Wrong: `box` = 1.
- `last_reviewed`: today.
- Append a line to `## Log`: `- YYYY-MM-DD <confidence> right|wrong`.
- If the card turned out to be wrong, ambiguous or not atomic, suggest a fix
  and let the user decide (they wrote it).

## End

A one-line summary: how many right, which ones were sure-but-wrong, and when
the next cards are due. Then back to **copilot** mode.
