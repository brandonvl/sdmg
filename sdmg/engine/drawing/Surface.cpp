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
#include "DrawEngine.h"
#include "engine\GameObject.h"

namespace sdmg {
	namespace engine {
		namespace drawing {
			Surface::Surface(const std::string path, SDL_Renderer *renderer, DrawEngine *drawEngine) : _sliceWidth(0), _sliceHeight(0), _renderWidth(0), _renderHeight(0), _drawEngine(drawEngine) {
				load(path, renderer);
			}

			Surface::Surface(const std::string path, SDL_Renderer *renderer, DrawEngine *drawEngine, const float sliceWidth, const float sliceHeight) : _sliceWidth(sliceWidth), _sliceHeight(sliceHeight), _renderWidth(sliceWidth), _renderHeight(sliceHeight), _drawEngine(drawEngine) {
				load(path, renderer);
			}

			Surface::Surface(const std::string path, SDL_Renderer *renderer, DrawEngine *drawEngine, const float sliceWidth, const float sliceHeight, const float renderWidth, const float renderHeight) : _sliceWidth(sliceWidth), _sliceHeight(sliceHeight), _renderWidth(renderWidth), _renderHeight(renderHeight), _drawEngine(drawEngine) {
				load(path, renderer);
			}

			Surface::Surface(const std::string path, SDL_Renderer *renderer, DrawEngine *drawEngine, const float sliceWidth, const float sliceHeight, const float renderWidth, const float renderHeight, AnimationType animationType) : _sliceWidth(sliceWidth), _sliceHeight(sliceHeight), _renderWidth(renderWidth), _renderHeight(renderHeight), _animationType(animationType), _drawEngine(drawEngine) {
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

						if (!_renderWidth) _renderWidth = surface->w;
						if (!_renderHeight) _renderHeight = surface->h;

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

			SDL_Texture *Surface::getSDLTexture(int sliceIndex, GameObject *gameObject, std::function<void()> anitimationCompletedCallback) {
				if (sliceIndex >= _maxSliceIndex) {
					switch (_animationType) {
					case AnimationType::HOLDLAST:
						sliceIndex = _maxSliceIndex - 1;
						break;
					case AnimationType::ONCE:
						sliceIndex = _maxSliceIndex - 1;
						if (anitimationCompletedCallback != nullptr) 
							anitimationCompletedCallback();
						break;
					default:
						_drawEngine->resetStep(gameObject);
						sliceIndex = sliceIndex - floor(sliceIndex / _maxSliceIndex) * _maxSliceIndex;
						break;
					}
				}

				return _textures[sliceIndex];
			}

			SDL_Texture *Surface::getSDLTexture(int sliceIndex, GameObject *gameObject) {
				return getSDLTexture(sliceIndex, gameObject, nullptr);
			}

			float Surface::getRenderWidth() { return _renderWidth; }
			float Surface::getRenderHeight() { return _renderHeight; }
		}
	}
}