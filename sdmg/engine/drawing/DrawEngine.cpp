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
#include <Box2D\Box2D.h>
#include "engine\GameObject.h"

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
				_surfaces = new std::map<std::string, Surface*>;
				_objectSurfaces = new std::map<GameObject*, Surface*>;
				_window = SDL_CreateWindow("SDMG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
				_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
			}

			void DrawEngine::draw(std::string key, Rectangle rect) {
				SDL_RenderCopy(_renderer, (*_surfaces)[key]->getSDLTexture(), NULL, &rect.toSDLRect());
			}

			void DrawEngine::draw(GameObject *gameObject, Rectangle rect) {
				SDL_RenderCopy(_renderer, (*_objectSurfaces)[gameObject]->getSDLTexture(), NULL, &rect.toSDLRect());
			}
			
			void DrawEngine::draw(std::string key, Rectangle rect, int slice) {
				Surface *surface = (*_surfaces)[key];
				SDL_RenderCopy(_renderer, surface->getSDLTexture(), &surface->getSliceRect(slice), &rect.toSDLRect());
			}

			void DrawEngine::draw(GameObject *gameObject, Rectangle rect, int slice) {
				Surface *surface = (*_objectSurfaces)[gameObject];
				SDL_RenderCopy(_renderer, surface->getSDLTexture(), &surface->getSliceRect(slice), &rect.toSDLRect());
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
		}
	}
}
