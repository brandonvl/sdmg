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

			Surface::Surface(const std::string path, SDL_Renderer *renderer, const float sliceWidth, const float sliceHeight) : _sliceWidth(sliceWidth), _sliceHeight(sliceHeight) {
				load(path, renderer);
			}

			Surface::~Surface() {
				SDL_DestroyTexture(_texture);
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
						_width = surface->w;
						_height = surface->h;
						_texture = SDL_CreateTextureFromSurface(renderer, surface);

						if (_sliceWidth > 0 && _sliceHeight > 0) {
							_maxSliceIndex = (surface->w / _sliceWidth) * (surface->h / _sliceHeight) - 1;
						}

						SDL_FreeSurface(surface);
					}
					else printf("Unable to load image %s\n", path.c_str());			
				}
			}

			SDL_Texture *Surface::getSDLTexture() {
				return _texture;
			}

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
		}
	}
}