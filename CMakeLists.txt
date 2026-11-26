# =============================================================================
# Luma - top-level build file
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
# VS" so cl.exe is found). In VS Code / Visual Studio, just pick the preset
# in the UI.
#
# Each preset gets its own folder, so Debug and Release never overwrite each
# other. Nothing is ever written outside build/, so deleting build/ is always
# a safe "start from scratch" button.
# =============================================================================


# --- Step 1: Minimum CMake version and the project itself --------------------
# 3.25 is needed for the SYSTEM keyword used in Step 5.
# LANGUAGES: SDL is written in C, our code is C++, so we need both compilers.
cmake_minimum_required(VERSION 3.25)
project(luma LANGUAGES C CXX)


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
set(SDL_SHARED OFF CACHE BOOL "" FORCE) # no libSDL3.so ...
set(SDL_STATIC ON CACHE BOOL "" FORCE) # ... link it into our exe instead
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE) # SDL's internal test helpers
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE) # SDL's example programs

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.20 # pinned: builds never change under you
    GIT_SHALLOW TRUE # download only this version, not history
    SYSTEM # treat SDL headers as system headers, so
    # OUR warning flags don't fire on THEIR code
)
FetchContent_MakeAvailable(SDL3) # download (if needed) + add_subdirectory
# Result: a target called SDL3::SDL3 that we can link against.


# --- Step 6: Settings shared by all of our own targets -----------------------
# A function so every game gets identical flags and you change them in one
# place. "PRIVATE" = only affects this target, not things that link to it.
option(LUMA_ASAN "Enable AddressSanitizer (catches out-of-bounds/use-after-free at runtime)" OFF)
# Turn on with the *-asan presets, or: -DLUMA_ASAN=ON

# MSVC: AddressSanitizer can't be combined with /RTC1 (MSVC's own runtime
# checks), which CMake adds to every Debug build by default. Strip it.
# This runs AFTER Step 5, so SDL keeps its /RTC1. That's fine: SDL isn't
# built with ASan, only our own targets are (see luma_defaults below).
if(MSVC AND LUMA_ASAN)
    string(REPLACE "/RTC1" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
    string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
endif()

function(luma_defaults target)
    target_link_libraries(${target} PRIVATE SDL3::SDL3)

    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W4 # high warning level (roughly -Wall -Wextra)
            # /GR- /EHs-c-  # MSVC spelling of -fno-rtti -fno-exceptions
        )
        if(LUMA_ASAN)
            # No /Zi needed here: CMake already adds it to Debug builds.
            target_compile_options(${target} PRIVATE /fsanitize=address)

            # An MSVC ASan exe loads clang_rt.asan_dynamic-x86_64.dll at
            # startup. That DLL lives next to cl.exe, which is only on PATH
            # inside a developer prompt. Launched any other way (Explorer,
            # VS Code, RemedyBG...) the exe dies before main() with
            # 0xC0000135 = STATUS_DLL_NOT_FOUND.
            # Fix: after each build, copy the DLL next to the exe. Windows
            # always looks in the exe's own folder first.
            cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH msvc_bin_dir)
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${msvc_bin_dir}/clang_rt.asan_dynamic-x86_64.dll"
                "$<TARGET_FILE_DIR:${target}>"
                COMMENT "Copying ASan runtime DLL next to ${target}")
        endif()
    else()
        target_compile_options(${target} PRIVATE
            -Wall -Wextra # the standard "please warn me" set
            -Wshadow # local variable hides another of the same name
            # -Wconversion  # implicit float<->int etc. Very strict/noisy;
            # try it once you're comfortable.

            # Enforce the "C-style C++" subset at compiler level (optional).
            # Uncomment if you want the compiler to reject these features:
            # -fno-exceptions -fno-rtti
        )
        if(LUMA_ASAN)
            target_compile_options(${target} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
            target_link_options(${target} PRIVATE -fsanitize=address,undefined)
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
    luma_defaults(${name})

    # Games are GUI apps: no console window on double-click.
    # Portable: CMake turns this into the right thing per platform.
    #   Windows: links with /SUBSYSTEM:WINDOWS. The OS then calls WinMain
    #            instead of main; SDL_main.h supplies that WinMain and
    #            forwards to your main(), so game code doesn't change.
    #            Side effect: stdout/std::cout go nowhere. Use SDL_Log,
    #            which also reaches the debugger's output window.
    #   Linux:   ignored (no such concept; a console appears only if you
    #            launch from one).
    #   macOS:   ignored; the analogue there is MACOSX_BUNDLE (.app).
    set_target_properties(${name} PROPERTIES WIN32_EXECUTABLE TRUE)
endfunction()


# --- Step 8: Shipping ---------------------------------------------------------
# luma_ship(00_window 0.1) adds a build target that makes the zip a player
# receives:
#
#   cmake --build build/windows-release --target ship_00_window
#   => build/windows-release/ship/00_window-v0.1.zip
#
# How it works, in two CMake concepts:
#   install()   declares WHAT belongs in a release and where it goes inside
#               it. Only the exe: no .pdb (debug symbols), no build junk.
#               COMPONENT tags it, so each game can be installed on its own.
#   --install   copies a component's files into a folder (the "stage").
# The ship target installs into a fresh stage folder, then zips it with
# CMake's built-in `-E tar`, so no zip tool needs to be installed.
#
# The version lives here, in the call. The GitHub release workflow
# (.github/workflows/build.yml) requires the git tag to be exactly the zip
# name (00_window-v0.1), so a tag and its zip can't disagree.
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/ship)

function(luma_ship target version)
    set(zip_name "${target}-v${version}")
    set(stage "${CMAKE_BINARY_DIR}/ship/${zip_name}")

    install(TARGETS ${target} RUNTIME DESTINATION . COMPONENT ${target})

    if(CMAKE_BUILD_TYPE STREQUAL "Release" AND NOT LUMA_ASAN)
        add_custom_target(ship_${target}
            # Start from an empty stage, so files from an old build can't
            # sneak into the zip.
            COMMAND ${CMAKE_COMMAND} -E rm -rf "${stage}"
            COMMAND ${CMAKE_COMMAND} --install "${CMAKE_BINARY_DIR}"
            --component ${target} --prefix "${stage}"
            # Zip the folder itself, so unzipping gives one tidy folder.
            COMMAND ${CMAKE_COMMAND} -E tar cf "${zip_name}.zip" --format=zip "${zip_name}"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/ship"
            DEPENDS ${target} # build the exe first if it's out of date
            COMMENT "Shipping ${CMAKE_BINARY_DIR}/ship/${zip_name}.zip"
            VERBATIM)
    else()
        # S4: players only get release builds. In any other preset the target
        # exists, but fails with an explanation instead of zipping a debug or
        # ASan build.
        add_custom_target(ship_${target}
            COMMAND ${CMAKE_COMMAND} -E echo
            "ship_${target}: only release builds ship. Use the windows-release preset."
            COMMAND ${CMAKE_COMMAND} -E false
            VERBATIM)
    endif()
endfunction()


# --- Step 9: Targets ----------------------------------------------------------
# Build sanity check: prints SDL version + which video/audio backends got
# compiled in, and whether it can reach the WSLg display/audio. No window.
add_executable(sdl_check tools/sdl_check.cpp)
luma_defaults(sdl_check) # also links SDL: without it, SDL3/SDL.h isn't found

# Feasibility check: can this machine create an OpenGL ES 3.0 context via
# SDL3? Needed from game 3 on (desktop and Raspberry Pi). Opens a hidden
# window, so unlike sdl_check it isn't run in CI (no GPU on the runner).
add_executable(gles_check tools/gles_check.cpp)
luma_defaults(gles_check)
# Stand-in so the shipping pipeline can be tested before game 0 exists.
# Remove once a real game ships.
# luma_ship(sdl_check 0.1)

# Your games. Uncomment a line once the folder exists with a .cpp file in it.
# Add luma_ship(<name> <version>) under a game once it's ready to ship.
add_game(00_window)
# add_game(01_pong)
