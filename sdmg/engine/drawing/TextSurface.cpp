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
			TextSurface::TextSurface(SDL_Renderer *renderer, std::string text, SDL_Color fgColor, SDL_Color bgColor, TTF_Font *font)
			{
				_text = text;
				_font = font;
				_foregroundColor = fgColor;
				_backgroundColor = bgColor;

				SDL_Surface *surface = TTF_RenderText_Shaded(_font, _text.c_str(), _foregroundColor, _backgroundColor);
				
				_renderHeight = surface->h;
				_renderWidth = surface->w;
				
				_texture = SDL_CreateTextureFromSurface(renderer, surface);

				SDL_FreeSurface(surface);
			}

			SDL_Texture* TextSurface::getSDLTexture() {
				return _texture;
			}

			TextSurface::~TextSurface()
			{
			}

			float TextSurface::getRenderWidth() { return _renderWidth; }
			float TextSurface::getRenderHeight() { return _renderHeight; }
		}
	}
}
