//
//
//  @ Project : SDMG
//  @ File Name : DrawEngine.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "DrawEngine.h"
#include "TextSurface.h"
#include "DynamicTextSurface.h"
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
				Surface *surface = new Surface(path, _renderer, this);
				// Add Surface to _surfaces map
				_surfaces->insert(std::pair<std::string, Surface*>(key, surface));
			}

			void DrawEngine::load(GameObject *gameObject, std::string path) {
				// Create new Surface from specified path
				Surface *surface = new Surface(path, _renderer, this);
				// Add Surface to _surfaces map
				_objectSurfaces->insert(std::pair<GameObject*, Surface*>(gameObject, surface));
			}

			void DrawEngine::loadMap(std::string key, std::string path, float sliceWidth, float sliceHeight) {
				// Create new Surface from specified path
				Surface *surface = new Surface(path, _renderer, this, sliceWidth, sliceHeight);
				// Add Surface to _surfaces map
				_surfaces->insert(std::pair<std::string, Surface*>(key, surface));
			}
			
			void DrawEngine::loadMap(GameObject* gameObject, std::string path, float sliceWidth, float sliceHeight) {
				// Create new Surface from specified path
				Surface *surface = new Surface(path, _renderer, this, sliceWidth, sliceHeight);
				// Add Surface to _surfaces map
				_objectSurfaces->insert(std::pair<GameObject*, Surface*>(gameObject, surface));
			}

			void DrawEngine::loadMap(MovableGameObject *gameObject, MovableGameObject::State state, std::string path, float sliceWidth, float sliceHeight) {
				loadMap(gameObject, state, path, sliceWidth, sliceHeight, sliceWidth, sliceHeight);
			}

			void DrawEngine::loadMap(MovableGameObject *gameObject, MovableGameObject::State state, std::string path, float sliceWidth, float sliceHeight, float scale) {
				loadMap(gameObject, state, path, sliceWidth, sliceHeight, sliceWidth * scale, sliceHeight * scale);
			}

			void DrawEngine::loadMap(MovableGameObject *gameObject, MovableGameObject::State state, std::string path, float sliceWidth, float sliceHeight, float scale, Surface::AnimationType animationType) {
				loadMap(gameObject, state, path, sliceWidth, sliceHeight, sliceWidth * scale, sliceHeight * scale, animationType);
			}

			void DrawEngine::loadMap(MovableGameObject *gameObject, MovableGameObject::State state, std::string path, float sliceWidth, float sliceHeight, float renderWidth, float renderHeight) {
				loadMap(gameObject, state, path, sliceWidth, sliceHeight, renderWidth, renderHeight, Surface::AnimationType::REPEAT);
			}

			void DrawEngine::loadMap(MovableGameObject *gameObject, MovableGameObject::State state, std::string path, float sliceWidth, float sliceHeight, float renderWidth, float renderHeight, Surface::AnimationType animationType) {
				std::map<MovableGameObject::State, Surface*> *surfaceMap;

				if (_objectStateSurfaces->count(gameObject)) surfaceMap = (*_objectStateSurfaces)[gameObject];
				else {
					surfaceMap = new std::map<MovableGameObject::State, Surface*>;
					_objectStateSurfaces->insert(std::make_pair(gameObject, surfaceMap));
				}

				// Create new Surface from specified path
				Surface *surface = new Surface(path, _renderer, this, sliceWidth, sliceHeight, renderWidth, renderHeight, animationType);
				// Add Surface to _surfaces map				
				surfaceMap->insert(std::make_pair(state, surface));
			}

			void DrawEngine::unload(std::string key) {
				if (_surfaces->find(key) != _surfaces->end()) {
					delete (*_surfaces)[key];
					_surfaces->erase(key);
				}
			}

			void DrawEngine::unloadText(std::string key) {
				if (_textSurfaces->find(key) != _textSurfaces->end()) {
					delete (*_textSurfaces)[key];
					_textSurfaces->erase(key);
				}
			}
			
			void DrawEngine::unloadAll() {
				std::map<std::string, Surface*>::iterator itr = _surfaces->begin();
				while (itr != _surfaces->end()) {
					delete itr->second;
					_surfaces->erase(itr++);
				}

				std::map<std::string, TextSurface*>::iterator textItr = _textSurfaces->begin();
				while (textItr != _textSurfaces->end()) {
					delete textItr->second;
					_textSurfaces->erase(textItr++);
				}

				std::map<GameObject*, Surface*>::iterator objectItr = _objectSurfaces->begin();
				while (objectItr != _objectSurfaces->end()) {
					delete objectItr->second;
					_objectSurfaces->erase(objectItr++);
				}

				std::map<MovableGameObject*, std::map<MovableGameObject::State, Surface*>*>::iterator objectStateItr = _objectStateSurfaces->begin();
				while (objectStateItr != _objectStateSurfaces->end()) {
					std::map<MovableGameObject::State, Surface*> *stateSurfaces = objectStateItr->second;
					std::map<MovableGameObject::State, Surface*>::iterator stateItr = stateSurfaces->begin();

					while (stateItr != stateSurfaces->end()) {
						delete stateItr->second;
						stateSurfaces->erase(stateItr++);
					}


					delete objectStateItr->second;
					_objectStateSurfaces->erase(objectStateItr++);
				}

				_steps->clear();
			}
			
			void DrawEngine::initialize() {
				_windowWidth = 1280;
				_windowHeight = 720;
				_surfaces = new std::map<std::string, Surface*>;
				_objectSurfaces = new std::map<GameObject*, Surface*>;
				_steps = new std::map<GameObject*, int>;
				_window = SDL_CreateWindow("SDMG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowWidth, _windowHeight, 0);
				//  _window = SDL_CreateWindow("SDMG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowWidth, _windowHeight, SDL_WINDOW_FULLSCREEN);
				_objectStateSurfaces = new std::map<MovableGameObject*, std::map<MovableGameObject::State, Surface*>*>;
				_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
				_textSurfaces = new std::map<std::string, TextSurface*>();
				_dynTextSurfaces = new std::map<std::string, DynamicTextSurface*>();
				_step = 1.0f / 15.0f;
				_lastUpdate = std::chrono::high_resolution_clock::now();

				SDL_ShowCursor(0);
			}

			void DrawEngine::draw(std::string key) {
				Surface *surface = (*_surfaces)[key];
				SDL_RenderCopy(_renderer, surface->getSDLTexture(), nullptr, nullptr);
			}

			void DrawEngine::draw(std::string key, float x, float y) {
				Surface *surface = (*_surfaces)[key];
				SDL_RenderCopy(_renderer, surface->getSDLTexture(), nullptr, &Rectangle(x, y, surface->getRenderWidth(), surface->getRenderHeight()).toSDLRect());
			}

			void DrawEngine::draw(GameObject *gameObject) {
				Surface *surface = (*_objectSurfaces)[gameObject];
				float x, y;
				calcXY(gameObject, surface, x, y);
				SDL_RenderCopy(_renderer, surface->getSDLTexture(), nullptr, &Rectangle(x, y, surface->getRenderWidth(), surface->getRenderHeight()).toSDLRect());
			}
			
			void DrawEngine::draw(std::string key, float x, float y, int slice) {
				Surface *surface = (*_surfaces)[key];
				SDL_RenderCopy(_renderer, surface->getSDLTexture(slice, nullptr), nullptr, &Rectangle(x, y, surface->getRenderWidth(), surface->getRenderHeight()).toSDLRect());
			}

			void DrawEngine::drawSlice(GameObject *gameObject) {
				createStep(gameObject);

				Surface *surface = (*_objectSurfaces)[gameObject];
				float x, y;
				calcXY(gameObject, surface, x, y);
				SDL_RenderCopy(_renderer, surface->getSDLTexture((*_steps)[gameObject], gameObject), nullptr, &Rectangle(x, y, surface->getRenderWidth(), surface->getRenderHeight()).toSDLRect());
			}

			void DrawEngine::drawSlice(MovableGameObject *gameObject) {
				drawSlice(gameObject, gameObject->getState(), gameObject->getDirection());
			}

			void DrawEngine::drawSlice(MovableGameObject *gameObject, MovableGameObject::State state, MovableGameObject::Direction direction) {
				createStep(gameObject);
				
				Surface *surface = (*(*_objectStateSurfaces)[gameObject])[state];
				float x, y;
				calcXY(gameObject, surface, x, y);

				std::function<void()> callback = std::bind(&MovableGameObject::stateCompleted, gameObject);
				SDL_RenderCopyEx(_renderer, surface->getSDLTexture((*_steps)[gameObject], gameObject, callback), nullptr, &Rectangle(x, y, surface->getRenderWidth(), surface->getRenderHeight()).toSDLRect(), 0, nullptr, gameObject->getDirection() == MovableGameObject::Direction::LEFT ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
			}

			void DrawEngine::drawRectangle(Rectangle rect, const Uint8 r, const Uint8 g, const Uint8 b) {
				drawRectangle(rect, r, g, b, 0);
			}

			void DrawEngine::drawRectangle(Rectangle rect, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
				SDL_SetRenderDrawColor(_renderer, r, g, b, a);
				SDL_RenderFillRect(_renderer, &rect.toSDLRect());
			}

			void DrawEngine::calcXY(GameObject *gameObject, Surface *surface, float &x, float &y) {
				float fixtureHeight = (((b2PolygonShape*)gameObject->getBody()->GetFixtureList()->GetShape())->GetVertex(2).y - ((b2PolygonShape*)gameObject->getBody()->GetFixtureList()->GetShape())->GetVertex(0).y) * 20.0f;
				x = gameObject->getPixelX() - (surface->getRenderWidth() / 2);
				y = gameObject->getPixelY() + (fixtureHeight / 2) - surface->getRenderHeight();
			}

			void DrawEngine::loadText(std::string key, std::string text, SDL_Color fgColor, std::string fontName, int fontSize) {
				// Initialize SDL_ttf library
				if (!TTF_WasInit())
					TTF_Init();
				// Create new font
				std::string path = "assets/fonts/";
				TTF_Font *font = TTF_OpenFont(path.append(fontName.append(".ttf")).c_str(), fontSize);
				// Create new TextSurface
				TextSurface *tSurface = new TextSurface(_renderer, text, fgColor, font);
				// Insert TextSurface
				_textSurfaces->insert(std::pair<std::string, TextSurface*>(key, tSurface));
				// Close font
				TTF_CloseFont(font);
			}

			void DrawEngine::loadDynamicText(std::string key, SDL_Color fgColor, std::string fontName, int fontSize) {
				// Initialize SDL_ttf library
				if (!TTF_WasInit())
					TTF_Init();
				// Create new font
				std::string path = "assets/fonts/";
				TTF_Font *font = TTF_OpenFont(path.append(fontName.append(".ttf")).c_str(), fontSize);
				// Create new DynamicTextSurface
				DynamicTextSurface *tSurface = new DynamicTextSurface(fgColor, font);
				// Insert DynamicTextSurface
				_dynTextSurfaces->insert(std::pair<std::string, DynamicTextSurface*>(key, tSurface));
			}

			void DrawEngine::drawDynamicText(std::string key, std::string text, float x, float y) {
				DynamicTextSurface *tSurface = (*_dynTextSurfaces)[key];
				SDL_RenderCopy(_renderer, tSurface->drawTexture(_renderer, text), NULL, &Rectangle(x, y, tSurface->getRenderWidth(), tSurface->getRenderHeight()).toSDLRect());
			}

			void DrawEngine::drawText(std::string key, float x, float y) {
				TextSurface *tSurface = (*_textSurfaces)[key];
				SDL_RenderCopy(_renderer, tSurface->getSDLTexture(), NULL, &Rectangle(x, y, tSurface->getRenderWidth(), tSurface->getRenderHeight()).toSDLRect());
			}

			void DrawEngine::destroyText(std::string key) {
				TextSurface *tSurface = (*_textSurfaces)[key];
				SDL_DestroyTexture(tSurface->getSDLTexture());
			}

			const std::array<float, 2> DrawEngine::getTextSize(std::string key) {
				if (_textSurfaces->count(key)) {
					std::array<float, 2> sizes = { (*_textSurfaces)[key]->getRenderWidth(), (*_textSurfaces)[key]->getRenderHeight() };
					return sizes;
				}
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
				b2Vec2 leftUpperPoint, rightBottomPoint;

				while (body) {
					SDL_Rect r;
					GameObject *object = static_cast<GameObject*>(body->GetUserData());

					leftUpperPoint = ((b2PolygonShape*)body->GetFixtureList()->GetShape())->GetVertex(0);
					rightBottomPoint = ((b2PolygonShape*)body->GetFixtureList()->GetShape())->GetVertex(2);
					leftUpperPoint += body->GetWorldCenter();
					rightBottomPoint += body->GetWorldCenter();

					r.x = leftUpperPoint.x * 20.0f;
					r.y = leftUpperPoint.y * 20.0f;

					r.w = (rightBottomPoint.x - leftUpperPoint.x) * 20.0f;
					r.h = (rightBottomPoint.y - leftUpperPoint.y) * 20.0f;
					
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

			void DrawEngine::update() {
				auto curTime = std::chrono::high_resolution_clock::now();
				float diff = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - _lastUpdate).count() / 1000.0f;

				_lastUpdate = curTime;
				_accumulator += diff;

				while (_accumulator > _step) {
					for each (std::pair<GameObject*, int> p in (*_steps)) {
						(*_steps)[p.first]++;
					}

					_accumulator -= _step;
				}
			}

			void DrawEngine::resetStep(GameObject *gameObject) {
				(*_steps)[gameObject] = 0;
			}

			void DrawEngine::createStep(GameObject *gameObject) {
				if (_steps->count(gameObject) == 0)
					_steps->insert(std::make_pair(gameObject, 0));
			}
		}
	}
}