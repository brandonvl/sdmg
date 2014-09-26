//
//
//  @ Project : SDMG
//  @ File Name : DrawEngine.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include <string>
#include <map>
#include "sdl\include\SDL.h"

namespace sdmg {
	namespace engine {
		class Engine;

		namespace drawing {
			class Surface;

			class DrawEngine {
			public:
				void loadSpriteMap(std::string key, std::string path);
				void loadStaticImage(std::string key, std::string path);
				void unload(std::string key);
				void unloadAll();
			private:
				Engine *_engine;
				SDL_Window *_window;
				SDL_Renderer *_renderer;
				std::map<std::string,Surface*> *_surfaces;
				void initialize();
			};
		}
	}
}