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
				Surface(const std::string path, SDL_Renderer *renderer);
				Surface(const std::string path, SDL_Renderer *renderer, const float sliceWidth, const float sliceHeight);
				Surface(const std::string path, SDL_Renderer *renderer, const float sliceWidth, const float sliceHeight, const float renderWidth, const float renderHeight);
				virtual ~Surface();

				//SDL_Surface* getSDLSurface();
				SDL_Rect getSliceRect(int sliceIndex);
				SDL_Texture *getSDLTexture();

				float getRenderHeight();
				float getRenderWidth();
			private:
				//SDL_Surface *_surface;
				SDL_Texture *_texture;
				float _width, _height, _renderWidth, _renderHeight;
				const float _sliceWidth, _sliceHeight;
				int _maxSliceIndex;

				void load(std::string path, SDL_Renderer *renderer);
			};
		}
	}
}