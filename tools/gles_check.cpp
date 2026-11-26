// gles_check - verifies the machine can give us an OpenGL ES 3.0 context
// through SDL3. Not part of any game. Run it on each new machine (desktop
// now, the Raspberry Pi at game 2) before relying on GLES in game 3.
//
// What it does:
//   1. Asks SDL for a GLES 3.0 context (instead of desktop OpenGL) on a
//      hidden window.
//   2. Prints what the driver actually gave us: vendor, renderer (which GPU
//      and driver), GL version string and GLSL version string.
//   3. Exits 0 on success, 1 on failure.
//
// Usage:
//   gles_check          use the platform's native path (WGL on Windows,
//                       EGL/GLX on Linux). On Windows this needs a driver
//                       that supports WGL_EXT_create_context_es2_profile.
//   gles_check egl      force SDL to load GLES through EGL instead. On
//                       Windows that means ANGLE (libEGL.dll + libGLESv2.dll
//                       next to the exe), the fallback if native fails.
//
// Game 3 will set up its own context in platform/: this file is only a
// feasibility check, written so it stays readable when you get there.

#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>

// We don't include any GL headers: we need exactly one GL function, so we
// define its type and the enum values ourselves (values from the GLES 3.0
// spec / Khronos gl3.h). A real renderer uses a loader for hundreds of these.
#define GL_VENDOR                   0x1F00
#define GL_RENDERER                 0x1F01
#define GL_VERSION                  0x1F02
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
// GL functions use the "APIENTRY" calling convention (__stdcall on 32-bit
// Windows; on x64 and ARM there's only one convention, so it's a no-op).
#if defined(_WIN32) && !defined(_WIN64)
#define GLAPIENTRY __stdcall
#else
#define GLAPIENTRY
#endif
typedef const unsigned char *(GLAPIENTRY *PFN_glGetString)(unsigned int name);

int main(int argc, char **argv)
{
    bool force_egl = argc > 1 && strcmp(argv[1], "egl") == 0;
    if (force_egl) {
        // Must be set before SDL_Init / window creation to take effect.
        SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init FAIL -> %s\n", SDL_GetError());
        return 1;
    }
    printf("Video driver: %s, path: %s\n", SDL_GetCurrentVideoDriver(),
           force_egl ? "forced EGL" : "native");

    // Context attributes must be set BEFORE creating the window: SDL picks
    // the window's pixel format with them in mind.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    int result = 1;
    SDL_Window *window = SDL_CreateWindow("gles_check", 64, 64,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    SDL_GLContext context = nullptr;
    if (!window) {
        printf("Window FAIL -> %s\n", SDL_GetError());
    } else if (!(context = SDL_GL_CreateContext(window))) {
        // The interesting failure: the driver refused a GLES 3.0 context.
        printf("GLES 3.0 context FAIL -> %s\n", SDL_GetError());
        if (!force_egl) printf("   try: gles_check egl (needs ANGLE DLLs)\n");
    } else {
        // Function pointers are only valid once a context is current.
        PFN_glGetString glGetString =
            (PFN_glGetString)SDL_GL_GetProcAddress("glGetString");
        if (!glGetString) {
            printf("glGetString not found -> %s\n", SDL_GetError());
        } else {
            printf("GLES 3.0 context OK\n");
            printf("  GL_VENDOR:   %s\n", glGetString(GL_VENDOR));
            printf("  GL_RENDERER: %s\n", glGetString(GL_RENDERER));
            printf("  GL_VERSION:  %s\n", glGetString(GL_VERSION));
            printf("  GLSL:        %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
            result = 0;
        }
    }

    // Destroy in reverse order of creation.
    if (context) SDL_GL_DestroyContext(context);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
    return result;
}
