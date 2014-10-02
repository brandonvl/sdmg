//
//
//  @ Project : SDMG
//  @ File Name : TextSurface.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "..\..\sdl\include\SDL.h"
#include "..\..\sdl\include\SDL_ttf.h"
#include <string>

namespace sdmg {
	namespace engine {
		namespace drawing {
			class TextSurface
			{

			public:
				TextSurface(SDL_Color fgColor, SDL_Color bgColor, std::string text, SDL_Renderer *renderer);
				virtual ~TextSurface();

				SDL_Texture* getSDLTexture();
			private:
				SDL_Texture *_texture;
				TTF_Font *_font;
				SDL_Color _foregroundColor;
				SDL_Color _backgroundColor;
			};
		}
	}
}

