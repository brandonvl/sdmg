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
#include "sdl\include\SDL_ttf.h"
#include "Rectangle.h"
#include <vector>
#include "engine\GameObject.h"
#include "engine\MovableGameObject.h"
#include "Surface.h"
#include <chrono>

class b2Body;

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
				void load(GameObject *gameObject, std::string path);
				void loadMap(std::string key, std::string path, float sliceWidth, float sliceHeight);
				void loadMap(GameObject *gameObject, std::string path, float sliceWidth, float sliceHeight);
				void loadMap(MovableGameObject *gameObject, MovableGameObject::State state, std::string path, float sliceWidth, float sliceHeight);
				void loadMap(MovableGameObject *gameObject, MovableGameObject::State state, std::string path, float sliceWidth, float sliceHeight, float scale);
				void loadMap(MovableGameObject *gameObject, MovableGameObject::State state, std::string path, float sliceWidth, float sliceHeight, float scale, Surface::AnimationType animationType);
				void loadMap(MovableGameObject *gameObject, MovableGameObject::State state, std::string path, float sliceWidth, float sliceHeight, float renderWidth, float renderHeight);
				void loadMap(MovableGameObject *gameObject, MovableGameObject::State state, std::string path, float sliceWidth, float sliceHeight, float renderWidth, float renderHeight, Surface::AnimationType animationType);
				void unload(std::string key);
				void unloadAll();

				void draw(std::string key);
				void draw(std::string key, float x, float y);
				void draw(GameObject *gameObject);
				void draw(std::string key, float x, float y, int slice);
				void drawSlice(GameObject *gameObject);
				//void draw(GameObject *gameObject, GameObject::State state, GameObject::Direction direction, float x, float y, int slice);
				void drawText(std::string text, Rectangle &rec, SDL_Color fgColor, SDL_Color bgColor, std::string font = "arial", int fontSize = 20);
				void drawSlice(MovableGameObject *gameObject);
				void drawSlice(MovableGameObject *gameObject, MovableGameObject::State state, MovableGameObject::Direction direction);
				void drawBodies(b2Body *body);
				void prepareForDraw();
				void render();
				void calcXY(GameObject *gameObject, Surface *surface, float &x, float &y);
				void update();
				void resetStep(GameObject *gameObject);
				void createStep(GameObject *gameObject);
				int getWindowHeight();
				int getWindowWidth();
			private:
				Engine *_engine;
				SDL_Window *_window;
				SDL_Renderer *_renderer;
				int _curRenderer;
				std::map<std::string, Surface*> *_surfaces;
				std::map<GameObject*, Surface*> *_objectSurfaces;
				std::map<MovableGameObject*, std::map<MovableGameObject::State, Surface*>*> *_objectStateSurfaces;
				void initialize();
				int _windowHeight;
				int _windowWidth;
				std::map<GameObject*, int> *_steps;

				std::chrono::high_resolution_clock::time_point _lastUpdate;
				float _step, _accumulator;
			};
		}
	}
}