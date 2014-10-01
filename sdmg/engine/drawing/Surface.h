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
#include <string>

namespace sdmg {
	namespace engine {
		namespace drawing {
			class Rectangle;

			class Surface {

			public:
				Surface(std::string path);
				virtual ~Surface();

				SDL_Surface* getSDLSurface();
			private:
				SDL_Surface *_surface;
				Rectangle *_rectangle;
			};
		}
	}
}