//
//
//  @ Project : SDMG
//  @ File Name : StaticSurface.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "Surface.h"
#include "sdl\include\SDL.h"

namespace sdmg {
	namespace engine {
		namespace drawing {
			class Rectangle;

			class StaticSurface : public Surface {
			public:
				void draw(SDL_Renderer *renderer, Rectangle *rectangle);
			};
		}
	}
}