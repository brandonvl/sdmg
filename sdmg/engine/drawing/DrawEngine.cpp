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
#include "engine\particle\Particle.h"
#include "..\..\sdl\include\SDL_image.h"

namespace sdmg {
	namespace engine {
		namespace drawing {
			DrawEngine::DrawEngine() {
				initialize();
			}

			DrawEngine::~DrawEngine() {
				unloadAll();
				TTF_Quit();
			}

			void DrawEngine::load(std::string key, std::string path) {
				// Create new Surface from specified path
				Surface *surface = new Surface(path, _renderer, this);
				// Add Surface to _surfaces map
				if (_surfaces.find(key) != _surfaces.end()) { delete _surfaces[key]; _surfaces[key] = surface; }
				else _surfaces.insert(std::pair<std::string, Surface*>(key, surface));
			}
			
			void DrawEngine::load(GameObject *gameObject, std::string path) {
				// Create new Surface from specified path
				Surface *surface = new Surface(path, _renderer, this);
				// Add Surface to _surfaces map
				if (_objectSurfaces.find(gameObject) != _objectSurfaces.end()) { delete _objectSurfaces[gameObject]; _objectSurfaces[gameObject] = surface; }
				_objectSurfaces.insert(std::pair<GameObject*, Surface*>(gameObject, surface));
			}

			void DrawEngine::loadMap(std::string key, std::string path, float sliceWidth, float sliceHeight) {
				// Create new Surface from specified path
				Surface *surface = new Surface(path, _renderer, this, sliceWidth, sliceHeight);
				// Add Surface to _surfaces map
				_surfaces.insert(std::pair<std::string, Surface*>(key, surface));
			}
			
			void DrawEngine::loadMap(GameObject* gameObject, std::string path, float sliceWidth, float sliceHeight) {
				// Create new Surface from specified path
				Surface *surface = new Surface(path, _renderer, this, sliceWidth, sliceHeight);
				// Add Surface to _surfaces map
				_objectSurfaces.insert(std::pair<GameObject*, Surface*>(gameObject, surface));
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

				if (_objectStateSurfaces.count(gameObject)) surfaceMap = _objectStateSurfaces[gameObject];
				else {
					surfaceMap = new std::map<MovableGameObject::State, Surface*>;
					_objectStateSurfaces.insert(std::make_pair(gameObject, surfaceMap));
				}

				if (surfaceMap->count(state))
					delete (*surfaceMap)[state];

				// Create new Surface from specified path
				Surface *surface = new Surface(path, _renderer, this, sliceWidth, sliceHeight, renderWidth, renderHeight, animationType);
				// Add Surface to _surfaces map				
				surfaceMap->insert(std::make_pair(state, surface));
			}

			void DrawEngine::copyMap(MovableGameObject *gameObject, MovableGameObject::State copyFrom, MovableGameObject::State copyTo) {
				if (_objectStateSurfaces.count(gameObject)) {
					auto surfaceMap = _objectStateSurfaces[gameObject];

					if (surfaceMap->count(copyFrom)) {
						surfaceMap->insert(std::make_pair(copyTo, surfaceMap->at(copyFrom)));
					}
				}
			}

			void DrawEngine::copyMap(std::string str, MovableGameObject *gameObject) {	
				std::map<MovableGameObject::State, Surface*> *surfaceMap;

				if (_objectStateSurfaces.count(gameObject)) surfaceMap = _objectStateSurfaces[gameObject];
				else {
					surfaceMap = new std::map<MovableGameObject::State, Surface*>;
					_objectStateSurfaces.insert(std::make_pair(gameObject, surfaceMap));
				}

				if (_surfaces.count(str)) {
					surfaceMap->insert(std::make_pair(MovableGameObject::State::IDLE, _surfaces[str]));
				}
			}

			void DrawEngine::unload(std::string key) {
				if (_surfaces.find(key) != _surfaces.end()) {
					delete _surfaces[key];
					_surfaces.erase(key);
				}
			}

			void DrawEngine::unloadText(std::string key) {
				if (_textSurfaces.find(key) != _textSurfaces.end()) {
					delete _textSurfaces[key];
					_textSurfaces.erase(key);
				}
			}
			
			void DrawEngine::unloadAll() {
				if (_surfaces.size() > 0) {
					std::map<std::string, Surface*>::iterator itr = _surfaces.begin();
					while (itr != _surfaces.end()) {
						delete itr->second;
						_surfaces.erase(itr++);
					}
				}

				if (_textSurfaces.size() > 0) {
					std::map<std::string, TextSurface*>::iterator textItr = _textSurfaces.begin();
					while (textItr != _textSurfaces.end()) {
						delete textItr->second;
						_textSurfaces.erase(textItr++);
					}
				}

				if (_objectSurfaces.size() > 0) {
					std::map<GameObject*, Surface*>::iterator objectItr = _objectSurfaces.begin();
					while (objectItr != _objectSurfaces.end()) {
						delete objectItr->second;
						_objectSurfaces.erase(objectItr++);
					}
				}

				std::vector<Surface*> v;

				if (_objectStateSurfaces.size() > 0) {
					std::map<MovableGameObject*, std::map<MovableGameObject::State, Surface*>*>::iterator objectStateItr = _objectStateSurfaces.begin();
					while (objectStateItr != _objectStateSurfaces.end()) {
						std::map<MovableGameObject::State, Surface*> *stateSurfaces = objectStateItr->second;
						std::map<MovableGameObject::State, Surface*>::iterator stateItr = stateSurfaces->begin();

						while (stateItr != stateSurfaces->end()) {
							if (std::find(v.begin(), v.end(), stateItr->second) == v.end()) {
								delete stateItr->second;
								v.push_back(stateItr->second);
							}
							
							stateSurfaces->erase(stateItr++);
						}


						delete objectStateItr->second;
						_objectStateSurfaces.erase(objectStateItr++);
					}
				}

				if (_dynTextSurfaces.size() > 0) {
					auto it = _dynTextSurfaces.begin();
					while (it != _dynTextSurfaces.end()) {
						delete it->second;
						_dynTextSurfaces.erase(it++);
					}
				}

				_steps.clear();
			}
			
			void DrawEngine::initialize() {
				_windowWidth = 1280;
				_windowHeight = 720;
				_window = SDL_CreateWindow("SDMG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowWidth, _windowHeight, 0);
				//_window = SDL_CreateWindow("SDMG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowWidth, _windowHeight, SDL_WINDOW_FULLSCREEN);
				_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
				_step = 1.0f / 15.0f;
				_lastUpdate = std::chrono::high_resolution_clock::now();

			}

			void DrawEngine::draw(std::string key) {
				Surface *surface = _surfaces[key];
				SDL_RenderCopy(_renderer, surface->getSDLTexture(), nullptr, nullptr);
			}

			void DrawEngine::draw(std::string key, float x, float y) {
				Surface *surface = _surfaces[key];
				SDL_RenderCopy(_renderer, surface->getSDLTexture(), nullptr, &Rectangle(x, y, surface->getRenderWidth(), surface->getRenderHeight()).toSDLRect());
			}

			void DrawEngine::draw(GameObject *gameObject) {
				if (_objectSurfaces.count(gameObject)) {

					Surface *surface = _objectSurfaces[gameObject];

					if (surface->isAnimated()) {
						drawSlice(gameObject);
					}
					else {
						float x, y;
						calcXY(gameObject, surface, x, y);
						SDL_RenderCopy(_renderer, surface->getSDLTexture(), nullptr, &Rectangle(x, y, surface->getRenderWidth(), surface->getRenderHeight()).toSDLRect());
					}
				}

				MovableGameObject *mGameObject = static_cast<MovableGameObject*>(gameObject);
				if (mGameObject && _objectStateSurfaces.count(mGameObject)) {
					draw(mGameObject);
				}
			}
			
			void DrawEngine::draw(std::string key, float x, float y, int slice) {
				Surface *surface = _surfaces[key];
				SDL_RenderCopy(_renderer, surface->getSDLTexture(slice, nullptr), nullptr, &Rectangle(x, y, surface->getRenderWidth(), surface->getRenderHeight()).toSDLRect());
			}

			void DrawEngine::drawSlice(GameObject *gameObject) {
				createStep(gameObject);

				Surface *surface = _objectSurfaces[gameObject];
				float x, y;
				calcXY(gameObject, surface, x, y);
				SDL_RenderCopy(_renderer, surface->getSDLTexture(_steps[gameObject], gameObject), nullptr, &Rectangle(x, y, surface->getRenderWidth(), surface->getRenderHeight()).toSDLRect());
			}

			void DrawEngine::draw(MovableGameObject *gameObject) {
				drawSlice(gameObject, gameObject->getState(), gameObject->getDirection());
			}

			void DrawEngine::drawSlice(MovableGameObject *gameObject, MovableGameObject::State state, MovableGameObject::Direction direction) {
				createStep(gameObject);
				
				Surface *surface = (*_objectStateSurfaces[gameObject])[state];
				float x, y;
				calcXY(gameObject, surface, x, y);

				std::function<void()> callback = std::bind(&MovableGameObject::stateCompleted, gameObject);
				SDL_RenderCopyEx(_renderer, surface->getSDLTexture(_steps[gameObject], gameObject, callback), nullptr, &Rectangle(x, y, surface->getRenderWidth(), surface->getRenderHeight()).toSDLRect(), 0, nullptr, gameObject->getDirection() == MovableGameObject::Direction::LEFT ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
			}

			void DrawEngine::drawRectangle(Rectangle rect, const Uint8 r, const Uint8 g, const Uint8 b) {
				drawRectangle(rect, r, g, b, 0);
			}

			void DrawEngine::drawRectangle(Rectangle rect, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
				SDL_SetRenderDrawColor(_renderer, r, g, b, a);
				SDL_RenderFillRect(_renderer, &rect.toSDLRect());
			}

			void DrawEngine::drawParticle(SDL_Surface *surface, int x, int y) {
				// Create texture & draw
				SDL_Texture* part = SDL_CreateTextureFromSurface(_renderer, surface);
				SDL_RenderCopy(_renderer, part, nullptr, &Rectangle(x, y, surface->w, surface->h).toSDLRect());
				// Destroy texture
				SDL_DestroyTexture(part);
			}

			void DrawEngine::refreshSurface(SDL_Surface *surface) {
				SDL_FillRect(surface, &surface->clip_rect, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
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

				if (_textSurfaces.find(key) != _textSurfaces.end()) { delete _textSurfaces[key]; _textSurfaces[key] = tSurface; }
				else _textSurfaces.insert(std::pair<std::string, TextSurface*>(key, tSurface));
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

				if (_dynTextSurfaces.find(key) != _dynTextSurfaces.end()) { delete _dynTextSurfaces[key]; _dynTextSurfaces[key] = tSurface; }
				else _dynTextSurfaces.insert(std::pair<std::string, DynamicTextSurface*>(key, tSurface));
			}

			void DrawEngine::drawDynamicText(std::string key, std::string text, float x, float y) {
				DynamicTextSurface *tSurface = _dynTextSurfaces[key];
				SDL_RenderCopy(_renderer, tSurface->drawTexture(_renderer, text), NULL, &Rectangle(x, y, tSurface->getRenderWidth(), tSurface->getRenderHeight()).toSDLRect());
			}

			void DrawEngine::drawText(std::string key, float x, float y) {
				TextSurface *tSurface = _textSurfaces[key];
				SDL_RenderCopy(_renderer, tSurface->getSDLTexture(), NULL, &Rectangle(x, y, tSurface->getRenderWidth(), tSurface->getRenderHeight()).toSDLRect());
			}

			void DrawEngine::destroyDynamicText(std::string key) {
				DynamicTextSurface *tSurface = _dynTextSurfaces[key];
				tSurface->destroyTexture();
			}

			void DrawEngine::destroyText(std::string key) {
				TextSurface *tSurface = _textSurfaces[key];
				SDL_DestroyTexture(tSurface->getSDLTexture());
			}

			const std::array<float, 2> DrawEngine::getTextSize(std::string key) {
				if (_textSurfaces.count(key)) {
					std::array<float, 2> size = { _textSurfaces[key]->getRenderWidth(), _textSurfaces[key]->getRenderHeight() };
					return size;
				}
			}

			const std::array<float, 2> DrawEngine::getImageSize(std::string key)
			{
				if (_surfaces.count(key))
				{
					std::array<float, 2> size = {_surfaces[key]->getRenderWidth(), _surfaces[key]->getRenderHeight() };
					return size;
				}
			}


			void DrawEngine::prepareForDraw() {
				if (!_preparing) {
					// clear renderer
					SDL_RenderClear(_renderer);
					_preparing = true;
				}
			}

			void DrawEngine::render() {
				if (_preparing) {
					// render
					SDL_RenderPresent(_renderer);
					_preparing = false;
				}
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

			void DrawEngine::drawHitBoxes(std::vector<input::Mouse::Hitbox*> &boxes) {

				for (auto box : boxes){
					SDL_Rect r;

					r.x = box->x;
					r.y = box->y;
					r.w = box->width;
					r.h = box->height;

					SDL_RenderFillRect(_renderer, &r);
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
				float diff = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - _lastUpdate).count() / 500.0f;

				_lastUpdate = curTime;
				_accumulator += diff;

				while (_accumulator > _step) {
					for each (std::pair<GameObject*, int> p in _steps) {
						_steps[p.first]++;
					}

					_accumulator -= _step;
				}
			}

			void DrawEngine::resetStep(GameObject *gameObject) {
				_steps[gameObject] = 0;
			}

			void DrawEngine::gameObjectStateChanged(MovableGameObject *gameObject) {
				resetStep(gameObject);
			}

			void DrawEngine::createStep(GameObject *gameObject) {
				if (_steps.count(gameObject) == 0)
					_steps.insert(std::make_pair(gameObject, 0));
			}

			void DrawEngine::createStep(MovableGameObject *gameObject) {
				if (_steps.count(gameObject) == 0) {
					_steps.insert(std::make_pair(gameObject, 0));
					gameObject->registerStateChangedCallback(std::bind(&DrawEngine::gameObjectStateChanged, this, gameObject));
				}
			}
		}
	}
}