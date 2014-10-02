//
//
//  @ Project : SDMG
//  @ File Name : Surface.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#include "Surface.h"
#include "Rectangle.h"
#include "..\..\sdl\include\SDL_image.h"

namespace sdmg {
	namespace engine {
		namespace drawing {
			Surface::Surface(const std::string path, SDL_Renderer *renderer) : _sliceWidth(0), _sliceHeight(0) {
				load(path, renderer);
			}

			Surface::Surface(const std::string path, SDL_Renderer *renderer, const float sliceWidth, const float sliceHeight) : _sliceWidth(sliceWidth), _sliceHeight(sliceHeight), _renderWidth(sliceWidth), _renderHeight(sliceHeight) {
				load(path, renderer);
			}

			Surface::Surface(const std::string path, SDL_Renderer *renderer, const float sliceWidth, const float sliceHeight, const float renderWidth, const float renderHeight) : _sliceWidth(sliceWidth), _sliceHeight(sliceHeight), _renderWidth(renderWidth), _renderHeight(renderHeight) {
				load(path, renderer);
			}

			Surface::~Surface() {
				for each (SDL_Texture *texture in _textures)
					SDL_DestroyTexture(texture);
			}

			void Surface::load(std::string path, SDL_Renderer *renderer) {
				// Initialize SDL_image with PNG loading flags
				int pngFlags = IMG_INIT_PNG;
				if (!(IMG_Init(pngFlags) & pngFlags))
					printf("SDL_image Error: %s\n", IMG_GetError());
				else
				{
					// Load image at specified path
					SDL_Surface *surface = IMG_Load(path.c_str());
					if (surface != NULL) {

						
						if (_sliceWidth > 0 && _sliceHeight > 0) {

							loadMap(surface, renderer);
						}
						else {

							_width = surface->w;
							_height = surface->h;
							_textures.push_back(SDL_CreateTextureFromSurface(renderer, surface));
						}

						if (_renderWidth == 0) _renderWidth = surface->w;
						if (_renderHeight == 0) _renderHeight = surface->h;

						SDL_FreeSurface(surface);
					}
					else printf("Unable to load image %s\n", path.c_str());			
				}
			}

			void Surface::loadMap(SDL_Surface *surface, SDL_Renderer *renderer) {
				SDL_Rect srcRect, dstRect;

				_maxSliceIndex = (surface->w / _sliceWidth) * (surface->h / _sliceHeight);

				dstRect.x = 0;
				dstRect.y = 0;
				dstRect.w = _sliceWidth;
				dstRect.h = _sliceHeight;

				int slicesPerRow = surface->w / _sliceWidth;
				
				for (int i = 0; i < _maxSliceIndex; i++) {
					SDL_Surface *tmpSurface = SDL_CreateRGBSurface(0, _sliceWidth, _sliceHeight, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);

					int rowIndex = floor(i / slicesPerRow);
					int colIndex = i - rowIndex * i;

					srcRect.x = _sliceWidth * colIndex;
					srcRect.y = _sliceHeight * rowIndex;
					srcRect.w = _sliceWidth;
					srcRect.h = _sliceHeight;

					SDL_BlitSurface(surface, &srcRect, tmpSurface, &dstRect);
					_textures.push_back(SDL_CreateTextureFromSurface(renderer, tmpSurface));

					SDL_FreeSurface(tmpSurface);
				}
			}

			SDL_Texture *Surface::getSDLTexture() {
				return _textures[0];
			}

			SDL_Texture *Surface::getSDLTexture(int sliceIndex) {
				if (sliceIndex > _maxSliceIndex) {
					sliceIndex = sliceIndex - floor(sliceIndex / _maxSliceIndex) * _maxSliceIndex;
				}

				return _textures[sliceIndex];
			}

			/*
			SDL_Rect Surface::getSliceRect(int sliceIndex) {
				SDL_Rect rect;

				if (_sliceWidth > 0 && _sliceHeight > 0) {

					if (sliceIndex > _maxSliceIndex) {
						sliceIndex = sliceIndex - floor(sliceIndex / _maxSliceIndex) * _maxSliceIndex;
					}
										
					int slicesPerRow = _width / _sliceWidth;
					int rowIndex = floor(sliceIndex / slicesPerRow);
					int colIndex = sliceIndex - rowIndex * slicesPerRow;
					
					rect.x = _sliceWidth * colIndex;
					rect.y = _sliceHeight * rowIndex;
					rect.w = _sliceWidth;
					rect.h = _sliceHeight;
					return rect;
				}
				else {
					rect.w = _width;
					rect.h = _height;
					return rect;
				}
			}
			*/

			float Surface::getRenderWidth() { return _renderWidth; }
			float Surface::getRenderHeight() { return _renderHeight; }
		}
	}
}