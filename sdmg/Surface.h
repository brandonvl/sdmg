#pragma once
#include "sdl\include\SDL.h"

namespace SDMG {
	class Size;

	class Surface
	{
	public:
		Surface();
		virtual ~Surface();
		void draw(SDL_Renderer *renderer, Size *size);

	protected:
		SDL_Surface _surface;
	};
}