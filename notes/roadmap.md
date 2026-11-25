# Roadmap (draft, for discussion)

Ten existing games, each chosen for what it **teaches** when you build it
without an engine. Each one is a real game you can play to get the "feel".
Designing your own twist is a stretch goal, never a requirement.

Built from: the 20 Games Challenge's structure (definition of done, stretch
goals, alternatives), the NYU CS-UY 3113 project sequence (Pong, then Lunar
Lander, then AI, then Platformer), the chapter order of Madhav's *Game
Programming in C++* (loop, then 2D objects, then vectors/physics, then AI,
then 3D, then audio), Becker & Parker's genre-to-concept mapping, and the
from-scratch gaps that engine-based lists skip.

These are private learning remakes. Don't publish them with the original
names or art.

## Per-game routine

1. **Play the original** (or watch footage) for 30 min. Write down the
   rules and how it feels.
2. **Write the definition of done** in the devlog *before* coding.
3. Build it. Time limit: roughly 2 weekends. When time runs out, ship what
   works.
4. **Retro** (5 min): what was new, what was hard, what should move to
   `core/`?
5. Optional **own-it side quest**: replace one thing SDL does for you.

## The ladder

| # | Game (alternative) | New learning outcomes | Own-it side quest |
|---|---|---|---|
| 0 | *Window + moving rect* | SDL setup, event loop, frame timing, drawing a rect | - |
| 1 | **Pong** | Game loop anatomy, input, delta time, AABB collision, scoring, reset | **Refactor:** split into platform/game layers |
| 2 | **Breakout** (Arkanoid) | Entity arrays, level layout as data, bounce angle from hit position, game states (serve/play/win/lose), save high score to file | Bitmap font text rendering |
| 3 | **Asteroids** (Lunar Lander) | Vector math + your vec2 type, rotation, thrust/drag, screen wrap, circle collision, particles, **fixed timestep** | Draw lines yourself (Bresenham) |
| 4 | **Tetris** (Snake) | Simulation on a discrete grid separate from rendering, rotation tables, input auto-repeat, timers/speed curve, menus | Memory arena: one block from the platform, no malloc in the game |
| 5 | **Space Invaders** (Galaga) | Loading images, sprite sheets, animation, many entities + pooling, formation movement, music + sound effects | Your own audio mixer |
| 6 | **Pac-Man** (Bomberman) | Tilemaps from file, grid-locked movement with turn buffering, AI state machines, per-ghost targeting, BFS | Debug overlay: draw AI targets/paths |
| 7 | **Super Mario Bros. world 1-1** (Pitfall, VVVVVV) | Platformer controller and how it feels, tile collision resolution, scrolling camera, enemies, level loading | Hot reload of game code or level data |
| 8 | **Worms** (Lemmings, Dig Dug) | Destructible pixel terrain, projectile physics, turn-based flow, camera follow/zoom, in-game UI | Software framebuffer: you write the pixels, SDL only presents them |
| 9 | **Wolfenstein 3D**, first level | Raycasting (DDA), first-person math, textured walls, sprites with a depth buffer: 3D on the CPU | Profile and optimize your rasterizer |
| 10 | **Super Monkey Ball**, a few levels (Star Fox rail section) | GPU pipeline (SDL_GPU or OpenGL), shaders, matrices, mesh loading, sphere-vs-triangle collision, 3D camera | - |

After 10: your own designs, or revisit a favourite with your own twist.

## Coverage by topic

| Topic | Introduced | Deepened |
|---|---|---|
| Game loop and time | 0, 1 | 3 (fixed timestep), 7 (hot reload) |
| Math | 3 (vec2) | 9 (raycasting), 10 (matrices) |
| Collision and physics | 1 (AABB) | 3 (circles), 7 (tiles), 8 (pixels), 10 (3D) |
| Rendering | 0 (rects) | 2 (text), 5 (sprites), 8 (software), 9 (CPU 3D), 10 (GPU) |
| Audio | 2 (sound effect) | 5 (mixing) |
| Data and content | 2 (level data) | 6 (tilemaps), 7 (level files) |
| AI | 1 (paddle AI) | 6 (state machines, pathing) |
| Architecture | 1 (platform split) | 4 (memory arena), 5 (pooling), 7 (hot reload) |
| UI and flow | 2 (game states) | 4 (menus), 8 (in-game UI) |

## Open questions

- Is 10 the right number? An 8-game version, closer to The Game Assembly:
  merge 4 into 2/3 and drop 9 (but 9 is the bridge from 2D to GPU 3D).
- Which alternatives look more fun? Swap them freely. The learning
  outcomes matter more than the specific game.
