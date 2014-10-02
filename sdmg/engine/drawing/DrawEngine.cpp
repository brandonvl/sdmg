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
#include "TextSurface.h"
#include <Box2D\Box2D.h>
#include "engine\GameObject.h"
#include "engine\MovableGameObject.h"

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
				Surface *surface = new Surface(path, _renderer);
				// Add Surface to _surfaces map
				_surfaces->insert(std::pair<std::string, Surface*>(key, surface));
			}

			void DrawEngine::load(GameObject *gameObject, std::string path) {
				// Create new Surface from specified path
				Surface *surface = new Surface(path, _renderer);
				// Add Surface to _surfaces map
				_objectSurfaces->insert(std::pair<GameObject*, Surface*>(gameObject, surface));
			}

			void DrawEngine::loadMap(std::string key, std::string path, float sliceWidth, float sliceHeight) {
				// Create new Surface from specified path
				Surface *surface = new Surface(path, _renderer, sliceWidth, sliceHeight);
				// Add Surface to _surfaces map
				_surfaces->insert(std::pair<std::string, Surface*>(key, surface));
			}
			
			void DrawEngine::loadMap(GameObject* gameObject, std::string path, float sliceWidth, float sliceHeight) {
				// Create new Surface from specified path
				Surface *surface = new Surface(path, _renderer, sliceWidth, sliceHeight);
				// Add Surface to _surfaces map
				_objectSurfaces->insert(std::pair<GameObject*, Surface*>(gameObject, surface));
			}

			void DrawEngine::loadMap(MovableGameObject *gameObject, MovableGameObject::State state, std::string path, float sliceWidth, float sliceHeight) {
				loadMap(gameObject, state, path, sliceWidth, sliceHeight, sliceWidth, sliceHeight);
			}

			void DrawEngine::loadMap(MovableGameObject *gameObject, MovableGameObject::State state, std::string path, float sliceWidth, float sliceHeight, float scale) {
				loadMap(gameObject, state, path, sliceWidth, sliceHeight, sliceWidth * scale, sliceHeight * scale);
			}

			void DrawEngine::loadMap(MovableGameObject *gameObject, MovableGameObject::State state, std::string path, float sliceWidth, float sliceHeight, float renderWidth, float renderHeight) {
				std::map<MovableGameObject::State, Surface*> *surfaceMap;

				if (_objectStateSurfaces->count(gameObject)) surfaceMap = (*_objectStateSurfaces)[gameObject];
				else {
					surfaceMap = new std::map<MovableGameObject::State, Surface*>;
					_objectStateSurfaces->insert(std::make_pair(gameObject, surfaceMap));
				}

				// Create new Surface from specified path
				Surface *surface = new Surface(path, _renderer, sliceWidth, sliceHeight, renderWidth, renderHeight);
				// Add Surface to _surfaces map				
				surfaceMap->insert(std::make_pair(state, surface));
			}

			void DrawEngine::unload(std::string key) {
				if (_surfaces->find(key) != _surfaces->end()) {
					delete (*_surfaces)[key];
					_surfaces->erase(key);
				}
			}
			
			void DrawEngine::unloadAll() {
				std::map<std::string, Surface*>::iterator itr = _surfaces->begin();
				while (itr != _surfaces->end()) {
					delete itr->second;
					_surfaces->erase(itr++);
				}
			}
			
			void DrawEngine::initialize() {
				_windowHeight = 1280;
				_windowWidth = 720;
				_surfaces = new std::map<std::string, Surface*>;
				_objectSurfaces = new std::map<GameObject*, Surface*>;
				_window = SDL_CreateWindow("SDMG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowHeight, _windowWidth, 0);
				_objectStateSurfaces = new std::map<MovableGameObject*, std::map<MovableGameObject::State, Surface*>*>;
				_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
			}

			void DrawEngine::draw(std::string key) {
				Surface *surface = (*_surfaces)[key];
				SDL_RenderCopy(_renderer, surface->getSDLTexture(), nullptr, nullptr);
			}

			void DrawEngine::draw(std::string key, float x, float y) {
				Surface *surface = (*_surfaces)[key];
				SDL_RenderCopy(_renderer, surface->getSDLTexture(), nullptr, &Rectangle(x, y, surface->getRenderWidth(), surface->getRenderHeight()).toSDLRect());
			}

			void DrawEngine::draw(GameObject *gameObject, float x, float y) {
				Surface *surface = (*_objectSurfaces)[gameObject];
				SDL_RenderCopy(_renderer, surface->getSDLTexture(), nullptr, &Rectangle(x, y, surface->getRenderWidth(), surface->getRenderHeight()).toSDLRect());
			}
			
			void DrawEngine::draw(std::string key, float x, float y, int slice) {
				Surface *surface = (*_surfaces)[key];
				SDL_RenderCopy(_renderer, surface->getSDLTexture(slice), nullptr, &Rectangle(x, y, surface->getRenderWidth(), surface->getRenderHeight()).toSDLRect());
			}

			void DrawEngine::draw(GameObject *gameObject, float x, float y, int slice) {
				Surface *surface = (*_objectSurfaces)[gameObject];
				SDL_RenderCopy(_renderer, surface->getSDLTexture(slice), nullptr, &Rectangle(x, y, surface->getRenderWidth(), surface->getRenderHeight()).toSDLRect());
			}

			void DrawEngine::draw(MovableGameObject *gameObject, MovableGameObject::State state, MovableGameObject::Direction direction, float x, float y, int slice) {
				Surface *surface = (*(*_objectStateSurfaces)[gameObject])[state];
				SDL_RenderCopyEx(_renderer, surface->getSDLTexture(slice), nullptr, &Rectangle(x, y, surface->getRenderWidth(), surface->getRenderHeight()).toSDLRect(), 0, nullptr, gameObject->getDirection() == MovableGameObject::Direction::LEFT ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
			}

			void DrawEngine::drawText(std::string text, Rectangle &rec) {
				TextSurface *tSurface = new TextSurface({ 1, 3, 14 }, { 0, 0, 255 }, text, _renderer);
				SDL_RenderCopy(_renderer, tSurface->getSDLTexture(), NULL, &rec.toSDLRect());
			}

			void DrawEngine::prepareForDraw() {
				// clear renderer
				SDL_RenderClear(_renderer);
			}

			void DrawEngine::render() {
				// render
				SDL_RenderPresent(_renderer);
			}

			void DrawEngine::drawBodies(b2Body *body) {
				b2Vec2 leftUpperPoint;

				while (body) {
					SDL_Rect r;
					GameObject *object = static_cast<GameObject*>(body->GetUserData());

					leftUpperPoint = ((b2PolygonShape*)body->GetFixtureList()->GetShape())->GetVertex(0);
					leftUpperPoint += body->GetWorldCenter();

					r.x = leftUpperPoint.x * 1.0f;
					r.y = leftUpperPoint.y * 1.0f;
					r.w = object->getWidth();
					r.h = object->getHeight();
					
					SDL_RenderFillRect(_renderer, &r);

					body = body->GetNext();
				}
			}

			int DrawEngine::getWindowHeight() {
				return _windowHeight;
			}

			int DrawEngine::getWindowWidth() {
				return _windowWidth;
			}
		}
	}
}
