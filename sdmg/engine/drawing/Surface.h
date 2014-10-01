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
				Surface(const std::string path);
				Surface(const std::string path, const float sliceWidth, const float sliceHeight);
				virtual ~Surface();

				SDL_Surface* getSDLSurface();
				SDL_Rect getSliceRect(int sliceIndex);
			private:
				SDL_Surface *_surface;
				const float _sliceWidth, _sliceHeight;
				int _maxSliceIndex;

				void load(std::string path);
			};
		}
	}
}