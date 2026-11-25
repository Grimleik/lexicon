# =============================================================================
# Lexicon - top-level build file
#
# Everyday commands (run from the repo root). The named setups ("presets")
# live in CMakePresets.json: {windows,linux}-{debug,release,asan}.
#
#   cmake --preset linux-debug             # CONFIGURE: reads this file,
#                                          # generates build/linux-debug/
#                                          # (run once, and again after you
#                                          # edit this file)
#   cmake --build build/linux-debug        # BUILD: compiles whatever changed
#   ./build/linux-debug/bin/sdl_check      # RUN: executables end up in bin/
#
# On Windows: same, with windows-debug (run from a "Developer PowerShell for
# VS 2022" so cl.exe is found). In VS Code / Visual Studio, just pick the
# preset in the UI.
#
# Each preset gets its own folder, so Debug and Release never overwrite each
# other. Nothing is ever written outside build/, so deleting build/ is always
# a safe "start from scratch" button.
# =============================================================================


# --- Step 1: Minimum CMake version and the project itself --------------------
# 3.25 is needed for the SYSTEM keyword used in Step 5.
# LANGUAGES: SDL is written in C, our code is C++, so we need both compilers.
cmake_minimum_required(VERSION 3.25)
project(lexicon LANGUAGES C CXX)


# --- Step 2: Language standard -----------------------------------------------
# C++20 mainly for designated initializers:  Rect r = { .x = 1, .y = 2 };
# EXTENSIONS OFF => -std=c++20 instead of -std=gnu++20, so the compiler tells
# you when you use a GCC-only feature (keeps the code portable to MSVC/Clang).
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# --- Step 3: Default build type ----------------------------------------------
# Debug = no optimization + debug symbols, which is what you want while
# learning (stepping through code in a debugger works properly).
# Only applies to single-config generators (Ninja, Makefiles); Visual Studio
# picks Debug/Release in the IDE instead, hence the CONFIGURATION_TYPES check.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()


# --- Step 4: Where outputs go + editor support -------------------------------
# Put every executable in build/bin instead of scattered sub-folders.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Writes build/compile_commands.json: a list of the exact compiler command for
# every file. Editors (clangd, VS Code C/C++) use it for correct
# autocomplete/go-to-definition.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# --- Step 5: Get SDL3 ---------------------------------------------------------
# FetchContent downloads SDL's source at configure time (into build/_deps/)
# and builds it as part of our project. Pros: no system install, everyone gets
# the exact same version, you can step into SDL's source in the debugger.
# Con: the first configure+build takes a few minutes.
#
# To upgrade SDL: change GIT_TAG, then `rm -rf build` and configure again.
# Available tags: https://github.com/libsdl-org/SDL/tags
#
# NOTE (Linux/WSL): SDL only enables features whose system dev packages are
# installed *when it is configured*. E.g. no libpulse-dev => no audio under
# WSLg. If you install packages later, `rm -rf build` and configure again.
include(FetchContent)

# SDL's own build options. CACHE ... FORCE makes them win over SDL's defaults.
set(SDL_SHARED       OFF CACHE BOOL "" FORCE)  # no libSDL3.so ...
set(SDL_STATIC       ON  CACHE BOOL "" FORCE)  # ... link it into our exe instead
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)  # SDL's internal test helpers
set(SDL_EXAMPLES     OFF CACHE BOOL "" FORCE)  # SDL's example programs

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.4.20   # pinned: builds never change under you
    GIT_SHALLOW    TRUE             # download only this version, not history
    SYSTEM                          # treat SDL headers as system headers, so
                                    # OUR warning flags don't fire on THEIR code
)
FetchContent_MakeAvailable(SDL3)    # download (if needed) + add_subdirectory
# Result: a target called SDL3::SDL3 that we can link against.


# --- Step 6: Settings shared by all of our own targets -----------------------
# A function so every game gets identical flags and you change them in one
# place. "PRIVATE" = only affects this target, not things that link to it.
option(LEXICON_ASAN "Enable AddressSanitizer (catches out-of-bounds/use-after-free at runtime)" OFF)
# Turn on with the *-asan presets, or: -DLEXICON_ASAN=ON

# MSVC: AddressSanitizer can't be combined with /RTC1 (MSVC's own runtime
# checks), which CMake adds to every Debug build by default. Strip it.
# Note: an ASan exe needs clang_rt.asan_dynamic-x86_64.dll at runtime; VS Code
# and Visual Studio find it, a plain double-click from Explorer may not.
if(MSVC AND LEXICON_ASAN)
    string(REPLACE "/RTC1" "" CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}")
    string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
endif()

function(lexicon_defaults target)
    target_link_libraries(${target} PRIVATE SDL3::SDL3)

    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W4             # high warning level (roughly -Wall -Wextra)
            # /GR- /EHs-c-  # MSVC spelling of -fno-rtti -fno-exceptions
        )
        if(LEXICON_ASAN)
            target_compile_options(${target} PRIVATE /fsanitize=address /Zi)
        endif()
    else()
        target_compile_options(${target} PRIVATE
            -Wall -Wextra   # the standard "please warn me" set
            -Wshadow        # local variable hides another of the same name
            # -Wconversion  # implicit float<->int etc. Very strict/noisy;
                            # try it once you're comfortable.

            # Enforce the "C-style C++" subset at compiler level (optional).
            # Uncomment if you want the compiler to reject these features:
            # -fno-exceptions -fno-rtti
        )
        if(LEXICON_ASAN)
            target_compile_options(${target} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
            target_link_options(${target}    PRIVATE -fsanitize=address,undefined)
        endif()
    endif()
endfunction()


# --- Step 7: Helper for adding a game -----------------------------------------
# add_game(01_pong) builds every .cpp under games/01_pong/ into an executable
# called 01_pong (=> build/bin/01_pong).
#
# GLOB collects the files for you; CONFIGURE_DEPENDS makes CMake re-check the
# folder on each build, so adding a new .cpp file "just works" without
# editing this file.
function(add_game name)
    file(GLOB_RECURSE sources CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/games/${name}/*.cpp)
    add_executable(${name} ${sources})
    lexicon_defaults(${name})
endfunction()


# --- Step 8: Targets ----------------------------------------------------------
# Build sanity check: prints SDL version + which video/audio backends got
# compiled in, and whether it can reach the WSLg display/audio. No window.
add_executable(sdl_check tools/sdl_check.cpp)
lexicon_defaults(sdl_check)

# Your games. Uncomment a line once the folder exists with a .cpp file in it.
# add_game(00_window)
# add_game(01_pong)
