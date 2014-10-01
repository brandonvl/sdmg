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
			Surface::Surface(const std::string path) : _sliceWidth(0), _sliceHeight(0) {
				load(path);
			}

			Surface::Surface(const std::string path, const float sliceWidth, const float sliceHeight) : _sliceWidth(sliceWidth), _sliceHeight(sliceHeight) {
				load(path);

				if (_surface) {
					_maxSliceIndex = (_surface->w / _sliceWidth) * (_surface->h / _sliceHeight) - 1;
				}
			}

			Surface::~Surface() {
				SDL_FreeSurface(_surface);
			}

			void Surface::load(std::string path) {
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

			SDL_Surface* Surface::getSDLSurface() {
				return _surface;
			}

			SDL_Rect Surface::getSliceRect(int sliceIndex) {
				SDL_Rect rect;

				if (_sliceWidth > 0 && _sliceHeight > 0) {

					if (sliceIndex > _maxSliceIndex) {
						sliceIndex = sliceIndex - floor(sliceIndex / _maxSliceIndex) * _maxSliceIndex;
					}
										
					int slicesPerRow = _surface->w / _sliceWidth;
					int rowIndex = floor(sliceIndex / slicesPerRow);
					int colIndex = sliceIndex - rowIndex * slicesPerRow;
					
					rect.x = _sliceWidth * colIndex;
					rect.y = _sliceHeight * rowIndex;
					rect.w = _sliceWidth;
					rect.h = _sliceHeight;
					return rect;
				}
				else {
					rect.w = _surface->w;
					rect.h = _surface->h;
					return rect;
				}
			}
		}
	}
}