// sdl_check - verifies the build setup, not part of any game.
//
// Prints which SDL version we built and which video/audio backends were
// compiled in, then tries to initialize video + audio to confirm SDL can
// reach the display and sound server (WSLg on WSL2). It does not open a window.

#include <SDL3/SDL.h>
#include <stdio.h>

int main(int, char**)
{
    int v = SDL_GetVersion();
    printf("SDL %d.%d.%d (%s)\n",
           SDL_VERSIONNUM_MAJOR(v), SDL_VERSIONNUM_MINOR(v), SDL_VERSIONNUM_MICRO(v),
           SDL_GetRevision());

    printf("Video backends compiled in:");
    for (int i = 0; i < SDL_GetNumVideoDrivers(); ++i) printf(" %s", SDL_GetVideoDriver(i));
    printf("\nAudio backends compiled in:");
    for (int i = 0; i < SDL_GetNumAudioDrivers(); ++i) printf(" %s", SDL_GetAudioDriver(i));
    printf("\n");

    // Check each subsystem separately so one failure doesn't hide the other.
    int failures = 0;
    if (SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        printf("Video OK  -> using '%s'\n", SDL_GetCurrentVideoDriver());
    } else {
        printf("Video FAIL -> %s\n", SDL_GetError());
        ++failures;
    }
    if (SDL_InitSubSystem(SDL_INIT_AUDIO)) {
        printf("Audio OK  -> using '%s'\n", SDL_GetCurrentAudioDriver());
    } else {
        printf("Audio FAIL -> %s\n", SDL_GetError());
        printf("   (on WSL: sudo apt install libpulse-dev, then rm -rf build and reconfigure)\n");
        ++failures;
    }
    SDL_Quit();
    return failures;
}
