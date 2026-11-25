## Blog
Definition of Done:
* Read about the game loop in game programming patterns.
	* Setup a basic game loop
* Move a rectangle on the screen using SDL Window and Input.
* Exit without any memory issues.

---

<!-- Review suggestions below (from Claude). Merge what you agree with into
     the sections above, reword in your own voice, delete the rest. -->

## Definition of Done: sharpened

Rule of thumb: every item should be something you can *check* by running
the program, not something you did. "Read X" is an activity, so it moves
to the Reading section.

- [ ] **Window:** opens, stays responsive, and closes cleanly with both the
      window's X button and Escape.
- [ ] **Rect:** moves with the keyboard (which keys? decide). Speed is
      defined in *pixels per second*, and it looks the same with vsync on
      and off.
- [ ] **Frame time:** the program measures how long each frame takes and
      reports it in a form you decide (e.g. avg/min/max in ms, printed
      once per second). This is the roadmap's "systems outcome" for
      game 0.
- [ ] **Memory:** "no memory issues" means, concretely:
  - runs and exits under the `windows-asan` preset with no reports;
  - everything you create, you destroy, in reverse order, then `SDL_Quit`;
  - leaks: MSVC's ASan does *not* check for leaks. Either run once under
    `linux-asan` in WSL (LeakSanitizer does), or look into the MSVC CRT
    debug heap (`_CrtDumpMemoryLeaks`) and decide if it's worth it.
- [ ] **Entry point:** chose normal `main()` vs SDL main callbacks, and
      wrote down why (see Decisions).
- [ ] **Time box:** ~1 weekend. When it runs out, ship what works.

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

## Reading

- [ ] Game Programming Patterns: [Game Loop](https://gameprogrammingpatterns.com/game-loop.html)
- [ ] [SDL3 README: main functions](https://wiki.libsdl.org/SDL3/README-main-functions)
- [ ] [SDL3 examples](https://examples.libsdl.org/SDL3/): renderer/clear, renderer/rectangles

## Decisions

<!-- One line each: what you chose, and why. E.g. entry point style, how
     you report frame time, which keys move the rect. -->

## Log

<!-- Dated notes while building: what broke, what surprised you. -->

## Retro

<!-- 5 min at the end: what was new, what was hard, what should move to
     core/ (nothing yet, it's game 0), answers to the questions above. -->
