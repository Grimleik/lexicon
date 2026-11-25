# Roadmap

Ten existing games, each one there for what it **teaches** when built
without an engine. Each is a real game you can play to get the "feel".
Your own design twist is a stretch goal, never a requirement.

The order puts small projects first, for momentum. The big games are cut
to a **slice** (one dungeon, one map, one chapter).

These are private learning remakes. Don't publish them with the original
names or art.

## Per-game routine

1. **Play the original** (or watch footage) for 30 min. Write down the
   rules and how it feels.
2. **Write the definition of done** in the devlog *before* coding: only
   things you can check by running the game. Also write **3-7 questions**
   this game should answer (drawn from its outcomes columns below).
3. **Read just in time:** only the "Read" column for this game, nothing
   ahead.
4. Build it. Time limit: small games ~1 weekend, slices ~3-4 weekends.
   When time runs out, ship what works.
5. **Retro** (5 min): what was new, what was hard, what should move to
   `core/`? Answer the questions from step 2, backed by something you
   measured or saw.
6. Optional **own-it side quest**: replace or build one system yourself.

## The ladder

| # | Game (slice) | Gameplay outcomes | Systems outcome | Own-it side quest |
|---|---|---|---|---|
| 0 | *Window + moving rect* | SDL event loop, drawing a rect | Frame timing: measure it | - |
| 1 | **Pong** | Loop, input, delta time, AABB collision, scoring | What a frame actually is | **Refactor:** split into platform/game layers |
| 2 | **Snake** | Grid movement, fixed update rate, growth, game over | Ring buffer, fixed timestep | Your own random number generator (xorshift/PCG) |
| 3 | **Breakout** | Entity arrays, levels as data, bounce angles, game states | Plain data files, save high score | Bitmap font text rendering |
| 4 | **Minesweeper** | Mouse input, flood fill, first click always safe, timer | Recursion vs an explicit stack (why deep recursion crashes) | Your own BMP loader |
| 5 | **Tetris** | Rotation tables, input auto-repeat, speed curve, menus | Memory arena: no `malloc` in game code | - |
| 6 | **Vampire Survivors** (one stage) | Swarms, auto-attacks, upgrade choices | **Performance:** profiling, spatial hashing, struct-of-arrays vs array-of-structs, cache behavior | Your own audio mixer (many sounds at once) |
| 7 | **Zelda**, NES-style (overworld region + one dungeon) | Tilemaps, screen transitions, many entity types, inventory, dialogue | Save game serialization, hot reload | WAV loader + asset pack file |
| 8 | **Fire Emblem** (one chapter) | Turn-based state machine, movement range, combat forecast, lots of UI | Dijkstra with terrain costs, AI by scoring options, unit data in files | Immediate-mode debug UI |
| 9 | **Diablo** (one generated dungeon level, one class) | Click-to-move, real-time combat, random loot, inventory grid | Isometric depth sorting, A*, procedural generation, light radius | Job system (threads) |
| 10 | **HoMM3** (small adventure map + hex battle) | Strategic layer, fog of war, daily economy, hex combat | Hex math; extract Fire Emblem's grid/turn code into `core/` | Your own Win32 window + input layer (replacing SDL's) |

**Alternatives** (swap freely; the outcomes matter more than the game):
Flappy Bird or Missile Command for 2-4 · Factorio-lite for 6 · Advance Wars
or Into the Breach for 8 · a Brogue-style roguelike as a warm-up before 9.

**After the ten (optional season 2: 3D):** a Doom-style software renderer
(BSP, all on the CPU), then a small game on the GPU (SDL_GPU or OpenGL).

## Read (just in time)

| # | Sources |
|---|---|
| 0-1 | [SDL3 examples](https://examples.libsdl.org/SDL3/) (official, runnable), [SDL3 wiki](https://wiki.libsdl.org/SDL3/), *Game Programming Patterns*: [Game Loop](https://gameprogrammingpatterns.com/game-loop.html) |
| 2 | Glenn Fiedler, [Fix Your Timestep!](https://gafferongames.com/post/fix_your_timestep/) |
| 3 | *Game Programming Patterns*: [State](https://gameprogrammingpatterns.com/state.html) |
| 4 | *CS:APP* ch. 3, the stack and procedure calls ([csapp.cs.cmu.edu](https://csapp.cs.cmu.edu/)) |
| 5 | Ryan Fleury, [Untangling Lifetimes: The Arena Allocator](https://www.rfleury.com/p/untangling-lifetimes-the-arena-allocator) |
| 6 | Casey Muratori, [Performance-Aware Programming](https://www.computerenhance.com/), *GPP*: [Data Locality](https://gameprogrammingpatterns.com/data-locality.html) and [Spatial Partition](https://gameprogrammingpatterns.com/spatial-partition.html), Mike Acton's "Data-Oriented Design and C++" talk (CppCon 2014) |
| 7 | Handmade Hero (same genre; dip into the relevant episodes, don't binge) |
| 8 | Red Blob Games, [Introduction to A*](https://www.redblobgames.com/pathfinding/a-star/introduction.html) (covers Dijkstra too) |
| 9 | Same A* page, [DevilutionX](https://github.com/diasurgical/devilutionX) (reconstructed Diablo 1 source) |
| 10 | Red Blob Games, [Hexagonal Grids](https://www.redblobgames.com/grids/hexagons/) |

**Background, not tied to a game:** *Computer Systems: A Programmer's
Perspective* (CS:APP), Ulrich Drepper,
[What Every Programmer Should Know About Memory](https://people.freebsd.org/~lstewart/articles/cpumemory.pdf)
(skim), [Fabien Sanglard](https://fabiensanglard.net/) (code reviews of
classic engines), [Handmade Network](https://handmade.network/).

## Coverage by topic

| Topic | Introduced | Deepened |
|---|---|---|
| Game loop and time | 0, 1 | 2 (fixed timestep), 7 (hot reload) |
| Memory | 2 (ring buffer) | 4 (stack), 5 (arena), 6 (cache/data layout) |
| Collision and spatial | 1 (AABB) | 3 (bounce), 6 (spatial hash), 7 (tiles) |
| Rendering (2D) | 0 (rects) | 3 (text), 4 (images), 7 (tilemaps), 9 (isometric) |
| Audio | 3 (sound effect) | 6 (mixing), 7 (own WAV loader) |
| Data and files | 3 (level data) | 7 (save games), 8 (unit data) |
| AI and pathfinding | 1 (paddle AI) | 8 (Dijkstra, scoring), 9 (A*), 10 (strategy) |
| Architecture | 1 (platform split) | 5 (arena), 7 (hot reload), 10 (extract to `core/`) |
| Platform | 0 (SDL) | 10 (own Win32 layer) |
| Performance | 0 (measure frame time) | 6 (profiling), 9 (threads) |
