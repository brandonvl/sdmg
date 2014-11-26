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
#include <array>
#include "engine\input\Mouse.h"

class b2Body;

namespace sdmg {
	namespace engine {
		class Engine;

		namespace drawing {
			class Surface;
			class TextSurface;
			class DynamicTextSurface;

			class DrawEngine {

			public:
				DrawEngine();
				virtual ~DrawEngine();
				void load(std::string key, std::string path);
				void load(GameObject *gameObject, std::string path);
				void loadText(std::string key, std::string text, SDL_Color fgColor, std::string fontName, int fontSize);
				void loadDynamicText(std::string key, SDL_Color fgColor, std::string fontName, int fontSize);
				void loadMap(std::string key, std::string path, float sliceWidth, float sliceHeight);
				void loadMap(GameObject *gameObject, std::string path, float sliceWidth, float sliceHeight);
				void loadMap(MovableGameObject *gameObject, MovableGameObject::State state, std::string path, float sliceWidth, float sliceHeight);
				void loadMap(MovableGameObject *gameObject, MovableGameObject::State state, std::string path, float sliceWidth, float sliceHeight, float scale);
				void loadMap(MovableGameObject *gameObject, MovableGameObject::State state, std::string path, float sliceWidth, float sliceHeight, float scale, Surface::AnimationType animationType);
				void loadMap(MovableGameObject *gameObject, MovableGameObject::State state, std::string path, float sliceWidth, float sliceHeight, float renderWidth, float renderHeight);
				void loadMap(MovableGameObject *gameObject, MovableGameObject::State state, std::string path, float sliceWidth, float sliceHeight, float renderWidth, float renderHeight, Surface::AnimationType animationType);
				void copyMap(MovableGameObject *gameObject, MovableGameObject::State copyFrom, MovableGameObject::State copyTo);
				void unload(std::string key);
				void unloadText(std::string key);
				void unloadAll();
				
				void draw(std::string key);
				void draw(std::string key, float x, float y);
				void draw(GameObject *gameObject);
				void draw(MovableGameObject *gameObject);
				void draw(std::string key, float x, float y, int slice);
				//void draw(GameObject *gameObject, GameObject::State state, GameObject::Direction direction, float x, float y, int slice);
				void drawText(std::string key, float x, float y);
				void drawDynamicText(std::string key, std::string text, float x, float y);
				const std::array<float, 2> DrawEngine::getTextSize(std::string key);
				const std::array<float, 2> DrawEngine::getImageSize(std::string key);
				void draw(MovableGameObject *gameObject, int slice);
				void draw(MovableGameObject *gameObject, MovableGameObject::State state, MovableGameObject::Direction direction, int slice);
				void destroyText(std::string key);
				void destroyDynamicText(std::string key);
				void drawBodies(b2Body *body);
				void drawHitBoxes(std::vector<input::Mouse::Hitbox*> &boxes);
				void drawRectangle(Rectangle rect, const Uint8 r, const Uint8 g, const Uint8 b);
				void drawRectangle(Rectangle rect, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a);
				void drawParticle(SDL_Surface *surface, int x, int y);
				void refreshSurface(SDL_Surface *surface);
				void prepareForDraw();
				void render();
				void calcXY(GameObject *gameObject, Surface *surface, float &x, float &y);
				void update();
				void resetStep(GameObject *gameObject);
				void gameObjectStateChanged(MovableGameObject *gameObject);
				void createStep(GameObject *gameObject);
				void createStep(MovableGameObject *gameObject);
				int getWindowHeight();
				int getWindowWidth();
			private:
				Engine *_engine;
				SDL_Window *_window;
				SDL_Renderer *_renderer;
				int _curRenderer;
				
				std::map<std::string, Surface*> _surfaces;
				std::map<std::string, TextSurface*> _textSurfaces;
				std::map<GameObject*, Surface*> _objectSurfaces;
				std::map<MovableGameObject*, std::map<MovableGameObject::State, Surface*>*> _objectStateSurfaces;
				std::map<std::string, DynamicTextSurface*> _dynTextSurfaces;
				
				void initialize();
				int _windowHeight;
				int _windowWidth;
				std::map<GameObject*, int> _steps;

				std::chrono::high_resolution_clock::time_point _lastUpdate;
				float _step, _accumulator;

				bool _preparing;

				void drawSlice(GameObject *gameObject);
				void drawSlice(MovableGameObject *gameObject, MovableGameObject::State state, MovableGameObject::Direction direction);
			};
		}
	}
}