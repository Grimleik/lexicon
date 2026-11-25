# Handoff: move lexicon to native Windows

**For:** the agent doing the Windows setup. **From:** the planning session
(2026-11-25, on WSL2 Ubuntu 24.04).

## Context (read first)

- This is a **learning project**: the user is learning game programming from
  scratch (C-style C++20 + SDL3) through 10 games of increasing difficulty
  (`notes/roadmap.md`).
- **Read `CLAUDE.md` and follow it.** The user writes all game, engine and
  platform code. Agents explain, give hints, and review. **Build files and
  tooling are the one exception:** you may write and fix those, with comments.
  Don't create anything under `games/`, `platform/` or `core/`.
- **Why the move:** WSLg distorts frame timing, vsync and audio latency;
  gamepads don't pass through; and the debuggers and profilers the user will
  want are on Windows. Linux stays a secondary target, so don't break it.

## Current state (verified on Linux)

| File | Purpose |
|---|---|
| `CMakeLists.txt` | Heavily commented, 8 numbered steps. SDL3 **3.4.20** via FetchContent (static, pinned). `lexicon_defaults()` sets warnings and ASan; `add_game(name)` globs `games/<name>/*.cpp`. |
| `CMakePresets.json` | `{windows,linux}-{debug,release,asan}`, Ninja, output in `build/<preset>/`. Each OS only sees its own presets (`condition` on `hostSystemName`). |
| `tools/sdl_check.cpp` | Build sanity check: prints the SDL version and backends, inits video and audio separately, exits non-zero on failure. No window. |
| `tools/lsan.supp` | Linux only: suppresses a known shutdown leak inside SDL's PulseAudio backend. |

Linux results: `linux-debug` and `linux-asan` configure, build and run
clean (`Video OK -> x11`, `Audio OK -> pulseaudio`). Reconfiguring doesn't
rebuild SDL.

## Untested: verify these on Windows

1. **Toolchain:** Visual Studio 2022 (Community or Build Tools) with the
   *Desktop development with C++* workload. It includes MSVC, CMake and Ninja.
   Also Git, and VS Code with **CMake Tools** + **C/C++**
   (`ms-vscode.cpptools`).
2. **Repo location:** on the Windows drive (e.g. `C:\dev\lexicon`), not under
   `\\wsl$`. Check that `.gitattributes` gives a clean checkout (no line-ending
   noise in `git status`).
3. **`windows-debug` preset**, both ways:
   - From a *Developer PowerShell for VS 2022*:
     `cmake --preset windows-debug`, then
     `cmake --build build/windows-debug`, then
     `build\windows-debug\bin\sdl_check.exe`.
     Expect video `windows` and an audio backend such as `wasapi`.
   - From VS Code: select the preset, build, then **CMake: Debug** on
     `sdl_check`. Breakpoints should hit, using the cppvsdbg debugger.
   - **Risk:** the preset relies on `architecture/toolset` with
     `"strategy": "external"` so that CMake Tools loads the MSVC developer
     environment for Ninja + `cl`. If `cl` isn't found when VS Code is started
     normally, fix it in the presets or VS Code settings, not by asking the
     user to always start VS Code from a dev prompt.
4. **`windows-asan` preset:** builds and runs. The `CMakeLists.txt` Step 6
   removes `/RTC1` and adds `/fsanitize=address /Zi`. Check:
   - no D9002/D8016 flag-conflict warnings;
   - the exe finds `clang_rt.asan_dynamic-x86_64.dll` when launched from VS
     Code. If it doesn't, set the debug environment/PATH (e.g. in
     `.vscode/settings.json` `cmake.debugConfig`) and document it.
5. **`windows-release`** builds.
6. **`/W4` on `sdl_check.cpp`:** should be warning-free. Fix anything that
   isn't.
7. **Linux still works:** don't change shared logic in a way that breaks the
   `linux-*` presets. If the user still has WSL, re-run `linux-debug` there.

## Constraints

- Keep the SDL pin (3.4.20), FetchContent and static linking unless
  something is broken. If you change one, say why.
- Keep the comment style in `CMakeLists.txt`: every non-obvious line explains
  itself. The user edits this file themselves.
- Don't add an SDL_main/WinMain setup or a GUI subsystem yet. `sdl_check` is
  a console tool, and how to set up the game entry point is the user's
  lesson in game 0. Mention it when it comes up, don't build it.

## Report back to the user

What you verified, what you changed and why (in plain terms, since they're
learning CMake), and anything still untested.

## Then

The user starts **game 0** (window + moving rect) by themselves, following
`notes/roadmap.md`. Optional extra: recommend a Windows debugger
(Visual Studio, RAD Debugger or RemedyBG) and leave the choice to the user.
