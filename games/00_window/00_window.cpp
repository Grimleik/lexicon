#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <cstdint>

typedef uint64_t u64;
typedef float f32;

struct AppState
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	u64 previous_frame;
	f32 delta;
	bool is_running = true;
};

struct GameState
{
	// TODO
};

static void GameState_Update(f32 delta)
{
	// TODO
}
static void GameState_Render(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColorFloat(renderer, 0.7f, 0.3f, 0.2f, 1.0f);

	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

int main(int argc, char **)
{
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to setup SDL.");
		return -1;
	}

	AppState app_state = {};
	if (!SDL_CreateWindowAndRenderer("Test",
									 800, 600, 0,
									 &app_state.window, &app_state.renderer))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to setup window and renderer.");
		return -1;
	}
	SDL_Event evt = {};

	while (app_state.is_running)
	{
		while (SDL_PollEvent(&evt))
		{
			if (evt.type == SDL_EVENT_QUIT)
			{
				app_state.is_running = false;
			}
		}

		u64 this_frame = SDL_GetTicks();
		app_state.delta = this_frame - app_state.previous_frame;
		app_state.previous_frame = this_frame;
		GameState_Update(app_state.delta / 1000.0f);
		GameState_Render(app_state.renderer);
	}

	SDL_DestroyRenderer(app_state.renderer);
	SDL_DestroyWindow(app_state.window);
	SDL_Quit();
	return 0;
}
