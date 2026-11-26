# Roadmap

Game remakes built without an engine, each there for what it **teaches**.
The ladder goes toward **graphics/rendering systems engineering**: rendering
moves from `SDL_Renderer` to my own code step by step
(`SDL_Renderer` → software 2D → OpenGL ES 3.0 → software 3D → GLES/Vulkan).

**Two target directions** (I pick one at the decision point after project A):

| Direction | What the work is | Must-have skills |
|---|---|---|
| Game rendering (engine render teams) | Render systems in an engine: particles, post, lighting, console profiling | C++, 3D math, shaders, Vulkan/DX12, GPU profiling, threads |
| Embedded graphics (camera / video devices) | Graphics in a camera's video stream on embedded SoCs: overlays, masks, image transforms | Embedded Linux C/C++, Python, GLES, OpenCL, image processing, perf on small hardware |

Structure: **4 × 2D games → 1 camera project → 2 × 3D games → capstone.**
Games stay pure games; project A is separate. **Raspberry Pi 4** is the
shared constrained target from the Pi bring-up (P) on. Rough total:
10–14 months.

These are private learning remakes. Don't publish them with the original
names or art. Portfolio write-ups describe them in my own words
("top-down swarm game, 5k entities at 60 fps on a Pi 4").

## Per-project routine

1. **Play the original** (or watch footage) for 30 min. Write down the
   rules and how it feels. (Project A: read the camera docs instead.)
2. **Draft the definition of done with `/plan`**, then make it yours,
   *before* coding: only things you can check by running it, plus
   **3-7 questions** drawn from the outcomes columns below.
   - From game 2 on (after the Pi bring-up), the DoD includes **"builds and runs on the Pi"** and
     **one measured number** on the Pi (frame time, fps, memory or
     bandwidth). Building natively on the Pi with the `linux-release`
     preset counts; cross-compiling is a CI nice-to-have (see backlog).
   - Projects marked *milestones* get one devlog (`notes/NN_name.md`) with
     a short DoD section per milestone.
3. **Read just in time:** only the "Read" row for this project.
4. **Build it on its own branch** (`git switch -c 01_pong`), pushing as you
   go so CI checks every push. When the time box runs out, ship what works.
   - Projects marked *milestones* are split into weekend-sized pieces, each
     with its own DoD, branch and PR.
5. **Ship:** PR into `master`, green CI, `/review`, merge, tag the merge
   commit (`git tag -a 02_breakout-v0.1 -m "..."`), push the tag.
   Milestones bump the version and name the milestone in the tag message
   (`06_diablo-v0.2`, message "m2: shadows"), so the release job still
   finds `ship_06_diablo`.
6. **Retro** (5 min): what was new, what was hard, what should move to
   `core/`? Answer the questions from step 2 with something measured or seen.
   - From game 3 on: **one RenderDoc capture** (desktop) and what it showed.
   - Turn the retro into a **blog post**: what I built, a screenshot or
     GIF, one measured number, links to the code at the release tag and to
     the download. Short is fine; this is practice in writing project
     write-ups, and the posts are the portfolio. (Site setup: see backlog.)
   - Then the **copilot setup**: what helped, what annoyed, anything to pull
     in from `notes/copilot_backlog.md`?
7. Optional **own-it side quest**: replace or build one system myself.

## The ladder

| # | Project (slice) | Renderer | Gameplay outcomes | Systems / graphics outcomes | Pi | Own-it side quest | Time box |
|---|---|---|---|---|---|---|---|
| 0 | *Window + moving rect* | SDL_Renderer | SDL event loop, drawing a rect | Measure frame time | - | - | 1 weekend |
| 1 | **Pong** | SDL_Renderer | Loop, input, delta time, AABB collision, scoring, paddle AI | Fixed timestep, what a frame is | - | **Refactor** into platform/game layers; own RNG (xorshift/PCG) | 1-2 weekends |
| P | *Pi bring-up* (tooling step, not a game) | - | n/a | Pi OS + SDL dev packages; native `linux-release` build; `sdl_check`, `gles_check`, then Pong running on the Pi; a way to get builds onto it (ssh/rsync or build there) | **First run on the Pi**; Pong's frame time on the Pi | - | ½ weekend |
| 2 | **Breakout**, *milestones* | m1: **own software 2D** (CPU pixel buffer → one texture upload per frame). m2: **OpenGL ES 3.0** (first GPU backend) | Entity arrays, levels as data, bounce angles, game states | m1: rects and blitting on a pixel buffer; arena allocator (no `malloc` in game code); save high score. m2: GLES context, shaders, textured quads; extract the thin **renderer interface** (two backends now exist); compare both backends' frame time | Runs (both backends) | Own BMP loader + bitmap font | m1: 2 weekends, m2: 1 weekend |
| 3 | **Vampire Survivors** (one stage), *milestones* | OpenGL ES 3.0 | Swarms, auto-attacks, upgrade choices | m1: sprite batching, instancing; **Dear ImGui** debug panel (live tuning, frame-time graph). m2: profiling, spatial hashing, SoA vs AoS, cache behavior | 60 fps with thousands of entities | - | 3 weekends |
| 4 | **Zelda-like** (overworld region + one dungeon), *milestones* | OpenGL ES 3.0 | Tilemaps, scrolling or screen transitions, many entity types, inventory, dialogue | GPU tilemaps, render targets, 2D lighting, bloom/post; save games; hot reload (shaders + assets) | Runs | WAV loader + asset pack file | 3-4 weekends |
| A | **Camera pipeline on the Pi** (not a game), *milestones* | CPU + GLES (+ OpenCL, or GLES 3.1 compute shaders if OpenCL on the Pi 4's V3D isn't usable; check when A starts); headless EGL | n/a | (1) raw Bayer → image on the CPU: black level, debayer, white balance, gamma; (2) same on the GPU; (3) live webcam YUV/NV12 → RGB in a shader; (4) privacy masks + overlays; (5) fisheye dewarp; (6) measure fps, latency, bandwidth | Pi only | Python script measuring image quality (noise, color error) | 3-4 weekends |
| 5 | **Doom-style arena** | **Software 3D rasterizer** (CPU) | First-person movement, doors, enemies | Transforms, clipping, z-buffer, perspective-correct texturing | Runs (CPU stress test) | BSP or portal culling | 3-4 weekends |
| 6 | **Diablo-like in 3D** (one generated level, one class), *milestones* | GLES 3.0, **or Vulkan if games won** | Click-to-move, real-time combat, random loot, inventory grid | m1: glTF loading + camera; m2: lighting + shadow maps; m3: gameplay + A* + procedural generation; m4: frustum culling, job system, Pi perf | Runs | Light radius as dynamic point lights | 6-8 weekends |
| 7 | **Capstone**, *milestones* | Rendering sandbox | Chosen at the decision point | **Games won:** Vulkan, PBR, HDR + tonemapping, deferred/clustered lighting, TAA, compute particles. **Embedded won:** full camera app on top of A (multiple streams, overlays, encoding, optional Yocto image) | Depends | - | ~2 months |

**Optional bonus list** (only if motivated): Fire Emblem (one chapter;
Dijkstra, scoring AI, heavy UI), HoMM3 (hex math, fog of war), a
Trackmania-lite racer (PBR/HDR showcase, offline path-traced photo mode),
Snake, Minesweeper, Tetris.

**Where the dropped games' lessons went:** fixed timestep → Pong; ring
buffer → input or audio code; BMP loader and arena → Breakout; recursion vs
explicit stack → flood fill in Diablo's level generator; Fire Emblem's
debug UI → Dear ImGui in game 3 (a requirement: every later project
reuses it for tuning); audio mixer → dropped; Win32 layer → headless EGL
(project A).

## Decision point (after project A)

Short retro over games 3, 4 and project A. One or two lines each:
1. Which one was hard to stop working on?
2. Which problems felt like "my kind of problem"?
3. Which result was most satisfying to show someone?
4. Which reading did I do voluntarily, beyond what was required?

The answers pick the capstone direction (and GLES vs Vulkan for game 6).

## Read (just in time)

| # | Sources |
|---|---|
| 0-1 | [SDL3 examples](https://examples.libsdl.org/SDL3/), [SDL3 wiki](https://wiki.libsdl.org/SDL3/), *Game Programming Patterns*: [Game Loop](https://gameprogrammingpatterns.com/game-loop.html), Glenn Fiedler, [Fix Your Timestep!](https://gafferongames.com/post/fix_your_timestep/) |
| 2 | [Tiny Renderer](https://haqr.eu/tinyrenderer/) (lines, triangles, barycentric lessons), *GPP*: [State](https://gameprogrammingpatterns.com/state.html), Ryan Fleury, [Untangling Lifetimes: The Arena Allocator](https://www.rfleury.com/p/untangling-lifetimes-the-arena-allocator) |
| 3 | [LearnOpenGL](https://learnopengl.com/) Getting Started + Instancing (written GLES 3.0-style), Casey Muratori, [Performance-Aware Programming](https://www.computerenhance.com/), *GPP*: [Data Locality](https://gameprogrammingpatterns.com/data-locality.html), [Spatial Partition](https://gameprogrammingpatterns.com/spatial-partition.html), Mike Acton, "Data-Oriented Design and C++" (CppCon 2014), [RenderDoc](https://renderdoc.org/) |
| 4 | LearnOpenGL: Framebuffers, Bloom, the 2D game chapters; Handmade Hero (tilemap episodes, dip in only) |
| A | [Raspberry Pi camera software docs](https://www.raspberrypi.com/documentation/computers/camera_software.html) (rpicam, raw capture, Picamera2), an IP-camera vendor's open-source native app SDK examples (OpenCL on video frames, overlays; reference only, pick one when A starts), [Mesa V3D driver docs](https://docs.mesa3d.org/drivers/v3d.html) |
| 5 | Tiny Renderer (remaining lessons), [Scratchapixel](https://www.scratchapixel.com/) rasterization, [Fabien Sanglard](https://fabiensanglard.net/) (Doom/Wolfenstein code reviews) |
| 6 | LearnOpenGL: Advanced Lighting / Shadow Mapping, Red Blob Games, [Introduction to A*](https://www.redblobgames.com/pathfinding/a-star/introduction.html); if Vulkan: [vkguide.dev](https://vkguide.dev/), [Khronos Vulkan Tutorial](https://docs.vulkan.org/tutorial/latest/) |
| 7 | [Real-Time Rendering resources](https://www.realtimerendering.com/), LearnOpenGL PBR, vkguide; offline: [Ray Tracing in One Weekend](https://raytracing.github.io/), [PBR 4th ed.](https://pbr-book.org/4ed/contents) |

**Background, not tied to a project:** *CS:APP* (ch. 3 for the stack),
Ulrich Drepper,
[What Every Programmer Should Know About Memory](https://people.freebsd.org/~lstewart/articles/cpumemory.pdf)
(skim), [3D Math Primer](https://gamemath.com/),
[Handmade Network](https://handmade.network/).

## Coverage by topic

| Topic | Introduced | Deepened |
|---|---|---|
| Game loop and time | 0, 1 (fixed timestep) | 4 (hot reload) |
| Memory | 2 (arena) | 3 (cache, data layout), 6 (job system) |
| Rendering: CPU | 2 (software 2D) | 5 (software 3D rasterizer) |
| Rendering: GPU | 2 m2 (GLES backend) | 3 (batching, instancing), 4 (render targets, post), 6 (3D lighting, shadows), 7 |
| Image processing | 4 (bloom/post as image ops) | A (camera pipeline) |
| Embedded / Pi | P (bring-up) | 3 (perf target), A (headless, camera), 6 |
| Collision and spatial | 1 (AABB) | 3 (spatial hash), 4 (tiles), 6 (A*) |
| AI and pathfinding | 1 (paddle AI) | 3 (swarm steering), 6 (A*, procedural generation) |
| Audio | 4 (WAV loader, played through SDL) | - |
| Tooling / debug UI | 3 (Dear ImGui panel) | 4 (hot reload), A, 6 |
| Data and files | 2 (levels, high score) | 4 (save games, asset pack), 6 (glTF) |
| Architecture | 1 (platform split) | 2 (arena; renderer interface once two backends exist), 4 (hot reload) |
| Platform | 0 (SDL3) | A (headless EGL) |
| Performance | 0 (frame time) | 3 (profiling), 6 (threads, Pi) |
