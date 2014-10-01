//
//
//  @ Project : SDMG
//  @ File Name : Surface.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#include "Surface.h"
#include "..\..\sdl\include\SDL_image.h"

namespace sdmg {
	namespace engine {
		namespace drawing {
			Surface::Surface(std::string path) {
				// Initialize SDL_image with PNG loading flags
				int pngFlags = IMG_INIT_PNG;
				if (!(IMG_Init(pngFlags) & pngFlags))
					printf("SDL_image Error: %s\n", IMG_GetError());
				else
				{
					// Load image at specified path
					_surface = IMG_Load(path.c_str());
					if (_surface == NULL)
						printf("Unable to load image %s\n", path.c_str());
				}
			}

			Surface::~Surface() {

			}

			SDL_Surface* Surface::getSDLSurface() {
				return _surface;
			}
		}
	}
}