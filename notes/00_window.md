## Definition of Done: sharpened

Rule of thumb: every item should be something you can *check* by running
the program, not something you did. "Read X" is an activity, so it moves
to the Reading section.

- [ ] **Window:** opens, stays responsive, and closes cleanly with both the
      window's X button and Escape.
- [ ] **Rect:** moves with the keyboard WASD. Speed is
      defined in *pixels per second*, and it looks the same with vsync on
      and off.
- [ ] **Frame time:** the program measures how long each frame takes and
      reports it in a form you decide, e.g. FPS and ms.
      This is the roadmap's "systems outcome" for game 0.
- [ ] **Memory:** "no memory issues" means, concretely:
  - runs and exits under the `windows-asan` preset with no reports;
  - everything you create, you destroy, in reverse order, then `SDL_Quit`;
  - leaks: MSVC's ASan does *not* check for leaks, try to run the MSVC CRT
    debug heap (`_CrtDumpMemoryLeaks`) and decide if it's worth it.
- [ ] **Entry point:** chose normal `main()` or SDL main callbacks, and
      wrote down why (see Decisions).
- [ ] **Shipping** (the zip is what a player receives: release build only):
  - [ ] **S1:** one command builds `00_window-v0.1.zip` from the
        `windows-release` preset (`--target ship_00_window`). *Why: if shipping is a chore, you won't
        do it. Set it up once, reuse it for all ten games.*
  - [ ] **S2:** unzipped into a fresh folder outside the repo, it runs on
        double-click. *Why: proves it doesn't depend on the build folder,
        working directory or terminal.*
  - [ ] **S3:** you know which DLLs the exe needs, checked with
        `dumpbin /dependents`. *Why: SDL is linked in statically, but the
        MSVC runtime (`VCRUNTIME140.dll`) probably isn't. That's what
        "dependencies" means in practice.*
  - [ ] **S4:** the zip has no debug or ASan builds, no `.pdb`, nothing
        the player doesn't need. *Why: ASan builds need extra DLLs and are
        slow; shipping one by mistake is a classic.*
  - [ ] **S5:** pushing a tag (`00_window-v0.1`) makes GitHub build the zip and
        attach it to a release; every push checks that the project builds.
        *Why: shipping and "does it build on a clean machine" become
        automatic. Tooling: generated with AI, explained, not hand-written.*
  - Stretch: run it in **Windows Sandbox** (clean Windows, built into
    Win 11 Pro). If the MSVC runtime is missing there, choose: static
    runtime (`/MT`) or ship the redistributable.
- [ ] **Time box:** ~1 weekend. When it runs out, ship what works. Use
  AI to help 'ship' the game to get into the habit of it.

## Questions to answer

Write a short answer for each in the Retro, ideally backed by something
you *measured* or *saw*, not something you read.

1. What is a frame? List what happens, in order, between two presents.
2. What happens if the loop stops handling events for a few seconds? Why
   does Windows care?
3. Moving "5 pixels per frame": what goes wrong on a 144 Hz monitor vs a
   60 Hz one? How does delta time fix it?
4. What does vsync do to your frame time and to CPU usage? Measure both,
   with vsync on and off (Task Manager is enough for CPU).
5. Which clock should you use for timing, and why? Compare
   `SDL_GetTicks` with `SDL_GetPerformanceCounter` / `SDL_GetTicksNS`:
   what are their resolutions?
6. What happens to your frame times when you drag the window, resize it,
   or minimize it?
7. Debug vs release build: is there a visible difference in frame time yet?
   Why or why not?
8. Which DLLs does your exe need at runtime, and which would a clean
   Windows install be missing? How did you find out? Then download your
   shipped game from its release page and play it.

## Reading

- [ ] Game Programming Patterns: [Game Loop](https://gameprogrammingpatterns.com/game-loop.html)
- [ ] [SDL3 README: main functions](https://wiki.libsdl.org/SDL3/README-main-functions)
- [ ] [SDL3 examples](https://examples.libsdl.org/SDL3/): renderer/clear, renderer/rectangles

## Retro

<!-- 5 min at the end: what was new, what was hard, what should move to
     core/ (nothing yet, it's game 0), answers to the questions above. -->
