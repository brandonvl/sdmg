//
//
//  @ Project : SDMG
//  @ File Name : TextSurface.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

namespace sdmg {
	namespace engine {
		namespace drawing {
			class TextSurface
			{

			public:
				TextSurface(SDL_Renderer *renderer, std::string text, SDL_Color fgColor, TTF_Font *font);
				virtual ~TextSurface();

				SDL_Texture* getSDLTexture();

				float getRenderWidth();
				float getRenderHeight();
			private:
				SDL_Texture *_texture;

				float _renderWidth;
				float _renderHeight;
			};
		}
	}
}

