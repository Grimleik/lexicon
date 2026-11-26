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

## Build / ship / CI

- **Automated tests:** unit tests once there's logic worth testing (Snake's
  ring buffer is the first candidate). Run them in the CI build job.
- **itch.io:** a restricted page (secret link) for games worth showing;
  upload from the release job with itch's `butler` CLI.
- **ASan in CI:** add `windows-asan` to the build matrix if minutes allow.
- **Linux + aarch64 in CI** (needed by game 2): a Linux job, then an
  aarch64 one (cross toolchain, or an ARM runner if the repo's plan allows
  it) so every push proves the Pi build still compiles. Needs SDL's dev
  packages on the runner first. Until then, build natively on the Pi.
- **Pi release zip:** `ship_` targets for `linux-release` on aarch64,
  attached to the same GitHub release as the Windows zip.
- **Project blog** (from the end of 00_window): GitHub Pages site in
  `docs/` with one post per project (screenshots/GIFs, code links at the
  release tag, download links). `tools/pages_check.sh` checks/enables it;
  `docs/index.md` is a placeholder. Code links and downloads need the repo
  public.

## Engine helpers

- **Shader hot-reload helper** (game 4): watch shader files, rebuild the
  program on change, keep the old one if compilation fails.
- **Headless EGL platform stub** (project A): a second `platform/`
  implementation that creates a GLES context without a window
  (EGL + GBM/surfaceless on the Pi).

## Session habits

- **End-of-session retrieval:** "3 things I learned today", written from
  memory, feeding the devlog and card drafts.
