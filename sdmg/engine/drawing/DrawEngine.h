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
#include "Rectangle.h"

namespace sdmg {
	namespace engine {
		class Engine;

		namespace drawing {
			class Surface;

			class DrawEngine {

			public:
				DrawEngine();
				virtual ~DrawEngine();
				void load(std::string key, std::string path);
				void unload(std::string key);
				void unloadAll();
				void draw(std::string key, Rectangle rect);
				void draw(std::string key, int slice, Rectangle rect, float sliceWidth, float sliceHeight);
			private:
				Engine *_engine;
				SDL_Window *_window;
				SDL_Renderer *_renderer;
				std::map<std::string, Surface*> *_surfaces;
				void initialize();
			};
		}
	}
}