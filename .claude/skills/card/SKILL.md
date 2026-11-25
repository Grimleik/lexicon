---
name: card
description: Review a ledger card the user drafted, help improve it, then file it in notes/ledger.
disable-model-invocation: true
---

The user has drafted one or more cards (the text after `/card`, or their
last message). Format: `notes/ledger/README.md`. Read it first. This is a
one-off action: don't change the current mode.

## Check each draft

Go through this list and report only what fails, briefly:

1. **Correct.** Any factual error must be fixed before filing. Explain it.
2. **Atomic.** One idea. If it holds more, propose a split.
3. **Unambiguous.** Only one reasonable answer to the question as written.
4. **Checkable.** The user can tell whether their answer was right.
5. **Understanding over parroting.** "What is X" is weak; "why X instead of
   Y", "what breaks without X", or "predict what happens" is stronger.
   Suggest a stronger phrasing if there is one.
6. **Right type.** Is this a fact or concept (`recall`), or a skill that is
   better practiced (`exercise`)?
7. **Answer is short.** If the answer is long, the card probably holds
   several ideas (see 2).

Then show the improved version **next to the user's original** and point out
what changed. They wrote it, so they decide: they can accept, edit, or keep
their own (unless it contains an error).

## File it

Once the user agrees, write the card to `notes/ledger/<slug>.md` using
`notes/ledger/_template.md`: `box: 1`, `created` and `last_reviewed` today,
tags as the user gave them, or suggest 1-2. Add a `## Read more` section
with 1-2 links to the exact section of a primary source that explains it
(a book chapter, the official docs), so a failed quiz has somewhere to go. The slug is a few words that
name the idea (`fixed-timestep-accumulator.md`). Confirm with the file path.
