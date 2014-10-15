//
//
//  @ Project : SDMG
//  @ File Name : TextSurface.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#include "TextSurface.h"
#include <string>

namespace sdmg {
	namespace engine {
		namespace drawing {
			TextSurface::TextSurface(SDL_Renderer *renderer, std::string text, SDL_Color fgColor, TTF_Font *font)
			{
				SDL_Color bgColor = { 0, 0, 0, 0 };
				SDL_Surface *surface = TTF_RenderText(font, text.c_str(), fgColor, bgColor);
				
				_renderHeight = surface->h;
				_renderWidth = surface->w;
				
				_texture = SDL_CreateTextureFromSurface(renderer, surface);


				SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_ADD);

				SDL_FreeSurface(surface);
			}

			SDL_Texture* TextSurface::getSDLTexture() {
				return _texture;
			}

			TextSurface::~TextSurface()
			{
				SDL_DestroyTexture(_texture);
			}

			float TextSurface::getRenderWidth() { return _renderWidth; }
			float TextSurface::getRenderHeight() { return _renderHeight; }
		}
	}
}
