# Luma

A series of small games and graphics projects written from scratch in C++
and SDL3, without an engine, to learn rendering and systems programming.
Rendering moves to my own code one step at a time:

`SDL_Renderer` → software 2D renderer → OpenGL ES 3.0 → software 3D rasterizer → OpenGL ES / Vulkan

Everything also targets a **Raspberry Pi 4**, so performance gets measured
on constrained hardware and not just a desktop GPU.

The goal is learning, not finishing quickly: I write the game, engine and
platform code myself. See [How the AI is used](#how-the-ai-is-used).

## Projects

Each project is a small remake, cut to a slice, chosen for what it teaches.
They are learning remakes inspired by existing games, using no original
names or art.

| # | Project | Renderer | Main lessons | Status |
|---|---|---|---|---|
| 0 | Window + moving rect | SDL_Renderer | Event loop, frame timing | in progress |
| 1 | Pong-style | SDL_Renderer | Fixed timestep, platform/game split | |
| P | Raspberry Pi bring-up | - | Native Pi build, first measurements | |
| 2 | Brick breaker | Software 2D, then GLES 3.0 | Pixel buffers, arena allocator, first GPU backend | |
| 3 | Swarm survival | GLES 3.0 | Instancing, profiling, data-oriented design | |
| 4 | Top-down adventure | GLES 3.0 | Tilemaps, render targets, post-processing, hot reload | |
| A | Camera pipeline (not a game) | CPU + GPU, headless | Raw Bayer → RGB, YUV, overlays, dewarp | |
| 5 | First-person arena | Software 3D | Transforms, clipping, z-buffer | |
| 6 | 3D dungeon crawler | GLES 3.0 or Vulkan | glTF, shadow maps, A*, job system | |
| 7 | Capstone | Rendering sandbox | Picked after project A | |

The full plan, with reading lists: [notes/roadmap.md](notes/roadmap.md).
One devlog per project lives in [notes/](notes/). Playable Windows builds
are on the [Releases](../../releases) page.

## Building

Needs CMake 3.25+, Ninja and a C++20 compiler. SDL3 is downloaded and
built automatically (pinned version, linked statically).

```sh
cmake --preset windows-debug          # or linux-debug; also -release, -asan
cmake --build build/windows-debug
build/windows-debug/bin/00_window
```

On Windows, run these from a *Developer PowerShell for VS*. All presets
are in [CMakePresets.json](CMakePresets.json), and
[CMakeLists.txt](CMakeLists.txt) is commented step by step.

Two check tools come with it: `sdl_check` (SDL version and backends) and
`gles_check` (can this machine create an OpenGL ES 3.0 context?).

## Layout

| Path | What |
|---|---|
| `games/NN_name/` | One folder per project |
| `tools/` | Check tools and scripts |
| `notes/` | Roadmap, devlogs, the learning ledger |
| `docs/` | Project blog (GitHub Pages) |
| `.claude/`, `CLAUDE.md` | AI copilot setup |

`platform/` (the only code that talks to SDL) and `core/` (shared code,
added only when a third project needs it) come in later.

## Code style

C-style C++20: structs, free functions, operator overloading for math,
`constexpr`, designated initializers. No inheritance, no STL containers,
no exceptions, no smart pointers. OpenGL code sticks to OpenGL ES 3.0 so
it runs unchanged on desktop and on the Pi.

## How the AI is used

I use Claude Code as a tutor, not an autopilot. [CLAUDE.md](CLAUDE.md)
sets the rules:

- **I write the code I want to learn from.** The AI answers questions,
  gives hints that get more specific only when I ask, and reviews my code
  without rewriting it.
- **Build files, CI and tooling** may be AI-written, with comments
  thorough enough that I can maintain them myself.
- **A spaced-repetition ledger** ([notes/ledger/](notes/ledger/)): I write
  a card when I learn something non-obvious, and the AI quizzes me on cards
  as they come due.
