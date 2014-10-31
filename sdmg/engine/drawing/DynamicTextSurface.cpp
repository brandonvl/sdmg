#include "DynamicTextSurface.h"

namespace sdmg {
	namespace engine {
		namespace drawing {
			DynamicTextSurface::DynamicTextSurface(SDL_Color fgColor, TTF_Font *font)
			{
				_bgColor = { 0, 0, 0, 0 };
				_fgColor = fgColor;
				_font = font;
			}

			SDL_Texture* DynamicTextSurface::drawTexture(SDL_Renderer *renderer, std::string text) {
				if (_texture) {
					SDL_DestroyTexture(_texture);
					SDL_FreeSurface(_surface);
				}
				_surface = TTF_RenderText(_font, text.c_str(), _fgColor, _bgColor);

				_renderHeight = _surface->h;
				_renderWidth = _surface->w;

				_texture = SDL_CreateTextureFromSurface(renderer, _surface);

				SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_ADD);

				return _texture;
			}

			DynamicTextSurface::~DynamicTextSurface() {
				SDL_DestroyTexture(_texture);
				TTF_CloseFont(_font);
			}

			float DynamicTextSurface::getRenderWidth() { return _renderWidth; }
			float DynamicTextSurface::getRenderHeight() { return _renderHeight; }
		}
	}
}
