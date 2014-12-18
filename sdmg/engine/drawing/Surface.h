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
#include <vector>
#include <functional>

namespace sdmg {
	namespace engine {
		class GameObject;

		namespace drawing {
			class Rectangle;
			class DrawEngine;

			class Surface {

			public:
				enum class AnimationType { ONCE, REPEAT, HOLDLAST };

				Surface(const std::string path, SDL_Renderer *renderer, DrawEngine *drawEngine);
				Surface(SDL_Surface *surface, SDL_Renderer *renderer, DrawEngine *drawEngine);
				Surface(const std::string path, SDL_Renderer *renderer, DrawEngine *drawEngine, const int sliceWidth, const int sliceHeight);
				Surface(const std::string path, SDL_Renderer *renderer, DrawEngine *drawEngine, const int sliceWidth, const int sliceHeight, const int renderWidth, const int renderHeight);
				Surface(const std::string path, SDL_Renderer *renderer, DrawEngine *drawEngine, const int sliceWidth, const int sliceHeight, const int renderWidth, const int renderHeight, AnimationType animationType);
				virtual ~Surface();

				//SDL_Surface* getSDLSurface();
				//SDL_Rect getSliceRect(int sliceIndex);
				SDL_Texture *getSDLTexture();
				SDL_Texture *getSDLTexture(int sliceIndex, GameObject *gameObject);
				SDL_Texture *getSDLTexture(int sliceIndex, GameObject *gameObject, std::function<void()> anitimationCompletedCallback);

				float getRenderHeight();
				float getRenderWidth();
				bool isAnimated();
				std::string getPath() { return _path; }
			private:
				//SDL_Surface *_surface;
				//SDL_Texture *_texture;
				int _width, _height, _renderWidth, _renderHeight;
				const int _sliceWidth, _sliceHeight;
				int _maxSliceIndex;
				std::vector<SDL_Texture*> _textures;
				AnimationType _animationType;
				DrawEngine *_drawEngine;

				void load(SDL_Renderer *renderer);
				void loadMap(SDL_Surface *surface, SDL_Renderer *renderer);
				std::string _path;
			};
		}
	}
}