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
			TextSurface::TextSurface(SDL_Color fgColor, SDL_Color bgColor, std::string text, SDL_Renderer *renderer, std::string font, int fontSize)
			{
				// Initialize SDL_ttf library
				if (!TTF_WasInit())
					TTF_Init();

				if (!_font)
					_font = TTF_OpenFont(font.append(".ttf").c_str(), fontSize);

				_foregroundColor = fgColor;
				_backgroundColor = bgColor;
 				SDL_Surface *surface = TTF_RenderText_Shaded(_font, text.c_str(), _foregroundColor, _backgroundColor);

				_texture = SDL_CreateTextureFromSurface(renderer, surface);

				SDL_FreeSurface(surface);
				TTF_CloseFont(_font);
			}

			SDL_Texture* TextSurface::getSDLTexture() {
				return _texture;
			}

			TextSurface::~TextSurface()
			{
			}
		}
	}
}
