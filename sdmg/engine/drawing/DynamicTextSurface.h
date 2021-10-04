//
//
//  @ Project : SDMG
//  @ File Name : DynamicTextSurface.h
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
			class DynamicTextSurface
			{
			public:
				DynamicTextSurface(SDL_Color fgColor, TTF_Font *font);
				virtual ~DynamicTextSurface();

				SDL_Texture* drawTexture(SDL_Renderer *renderer, std::string text);
				void destroyTexture();

				float getRenderWidth();
				float getRenderHeight();
			private:
				SDL_Surface *_surface = nullptr;
				SDL_Texture *_texture = nullptr;
				TTF_Font *_font = nullptr;
				SDL_Color _fgColor;
				SDL_Color _bgColor;

				float _renderWidth;
				float _renderHeight;
			};
		}
	}
}
