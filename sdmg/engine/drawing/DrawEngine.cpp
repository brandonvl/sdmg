//
//
//  @ Project : SDMG
//  @ File Name : DrawEngine.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "DrawEngine.h"
#include "Surface.h"

namespace sdmg {
	namespace engine {
		namespace drawing {
			DrawEngine::DrawEngine() {
				initialize();
			}

			DrawEngine::~DrawEngine() {

			}

			void DrawEngine::load(std::string key, std::string path) {
				// Create new Surface from specified path
				Surface *surface = new Surface(path);
				// Add Surface to _surfaces map
				_surfaces->insert(std::pair<std::string, Surface*>(key, surface));
			}
			
			void DrawEngine::unload(std::string key) {
				if (_surfaces->find(key) != _surfaces->end())
					_surfaces->erase(key);
			}
			
			void DrawEngine::unloadAll() {
				std::map<std::string, Surface*>::iterator itr = _surfaces->begin();
				while (itr != _surfaces->end()) {
					_surfaces->erase(itr++);
				}
			}
			
			void DrawEngine::initialize() {
				_surfaces = new std::map<std::string, Surface*>();
				_window = SDL_CreateWindow("SDMG", 0, 0, 1280, 720, SDL_WINDOW_RESIZABLE);
				_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
			}

			void DrawEngine::draw(std::string key, Rectangle rect) {
				Surface *surface = _surfaces->at(key);
				// Create texture from surface
				SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface->getSDLSurface());
				//SDL_FreeSurface(surface->getSDLSurface());

				// Render the current animation
				SDL_RenderCopy(_renderer, texture, NULL, NULL);
				SDL_RenderPresent(_renderer);
				SDL_Delay(500);
			}
			
			void DrawEngine::draw(std::string key, int slice, Rectangle rect, float sliceWidth, float sliceHeight) {

			}
		}
	}
}
