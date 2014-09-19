#include "sdl\include\SDL.h"
#pragma once

namespace SDMG {
	class Engine
	{
		class GameObject;

	public:
		Engine();
		virtual ~Engine();

	private:
		SDL_Window *_window;
		SDL_Renderer *_renderer;
		void create();
		void draw(GameObject* gameObject);
	};
}