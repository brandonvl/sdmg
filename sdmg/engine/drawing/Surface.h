//
//
//  @ Project : SDMG
//  @ File Name : Surface.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "..\..\sdl\include\SDL.h"

namespace sdmg {
	namespace engine {
		namespace drawing {
			class Rectangle;

			class Surface {
			public:
				virtual void draw(SDL_Renderer *renderer, Rectangle *rectangle) = 0;
			private:
				SDL_Surface _surface;
				Rectangle *_rectangle;
			};
		}
	}
}