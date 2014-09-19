#include "Engine.h"
#include "GameObject.h"

namespace SDMG {
	Engine::Engine()
	{
		create();
	}


	Engine::~Engine()
	{
	}

	void Engine::create() {
		_window = SDL_CreateWindow("SDMG", 0, 0, 1280, 720, SDL_WINDOW_FULLSCREEN);
		_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	}
}