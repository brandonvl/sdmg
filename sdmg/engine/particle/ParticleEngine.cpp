//
//
//  @ Project : SDMG
//  @ File Name : ParticleEngine.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#include "ParticleEngine.h"
#include "ParticleSet.h"
#include "ParticleInstance.h"
#include <SDL_image.h>
#include "engine\MovableGameObject.h"
#include <ctime>
#include <string>

namespace sdmg {
	namespace engine {
		namespace particle {
			ParticleEngine::ParticleEngine() {
				// Seed random generator
				srand(time(0));

				// Load particle images
				loadParticles();
			}

			ParticleEngine::~ParticleEngine() {
				unloadAll();
			}

			void ParticleEngine::loadParticles() {
				int pngFlags = IMG_INIT_PNG;
				if ((IMG_Init(pngFlags) & pngFlags)) {
					_particleImages.insert(std::make_pair("blood", IMG_Load("assets/particles/blood_4x4.png")));
					_particleImages.insert(std::make_pair("red", IMG_Load("assets/particles/red.png")));
					_particleImages.insert(std::make_pair("burst", IMG_Load("assets/particles/burst.png")));
					//_particleImages.insert(std::make_pair("yellow", IMG_Load("assets/particles/yellow.png")));

					for (auto p : _particleImages)
						SDL_SetSurfaceAlphaMod(p.second, SDL_ALPHA_OPAQUE);
				}
			}

			void ParticleEngine::createParticleSet(std::string key, int max, int x, int y, int xVel, int yVel, int width, int height, std::string image) {
				ParticleSet *ps = new ParticleSet(max, x, y, xVel, yVel, width, height, _particleImages[image]);
				_particleSets.insert(std::make_pair(key, ps));
			}

			void  ParticleEngine::continuousShowParticleSet(std::string key) {
				_particleSets[key]->showContinuous();
			}

			void  ParticleEngine::continuousShowParticleSet(ParticleInstance* instance) {
				instance->getParticleSet()->showContinuous();
			}

			void ParticleEngine::resetParticleLifeTime(std::string key) {
				_particleSets[key]->resetLifeTime();
			}

			void ParticleEngine::resetParticleLifeTime(ParticleInstance* instance) {
				instance->getParticleSet()->resetLifeTime();
			}

			void ParticleEngine::resetParticleSet(std::string key) {
				_particleSets[key]->reset();
			}

			void ParticleEngine::resetParticleSet(ParticleInstance* instance) {
				instance->getParticleSet()->reset();
			}

			void ParticleEngine::showParticleSet(ParticleInstance* instance) {
				instance->getParticleSet()->show();
			}

			SDL_Surface* ParticleEngine::getParticleSetSurface(ParticleInstance* instance) {
				return instance->getParticleSet()->getSDLSurface();
			}

			SDL_Surface* ParticleEngine::getParticleSetSurface(std::string key) {
				return _particleSets[key]->getSDLSurface();
			}

			void ParticleEngine::gameObjectStateChange(MovableGameObject *gameObject) {
				if (gameObject->getState() == MovableGameObject::State::RESPAWN && gameObject->getHP() > 0) {
					std::string key = "fall";
					SDL_Surface* surface = _particleSets[key]->getSDLSurface();

					int xVel = _particleSets[key]->getXVel();
					int yVel = _particleSets[key]->getYVel();

					int x = (gameObject->getPixelX() - (surface->w / 2));
					int y = 400;
					ParticleInstance *instance = new ParticleInstance(_particleSets[key], x, y, xVel, yVel);

					instance->getParticleSet()->reset();
					_drawContainer.push_back(instance);
				}
			}

			void ParticleEngine::gameObjectHit(MovableGameObject *gameObject) {
				std::string key = "hit";
				SDL_Surface* surface = _particleSets[key]->getSDLSurface();

				int xVel = _particleSets[key]->getXVel();
				int yVel = _particleSets[key]->getYVel();

				switch (MovableGameObject::State(gameObject->getState()))
				{
				case MovableGameObject::State::KNOCKBACKRIGHT:
					xVel = 13;
					yVel = 5;
					break;
				case MovableGameObject::State::KNOCKBACKLEFT:
					xVel = -5;
					yVel = 5;
					break;
				}

				int x = (gameObject->getPixelX() - (surface->w / 2));
				int y = (gameObject->getPixelY() - (surface->h / 2));
				ParticleInstance *instance = new ParticleInstance(_particleSets[key], x, y, xVel, yVel);

				instance->getParticleSet()->reset();
				_drawContainer.push_back(instance);
			}

			void ParticleEngine::registerGameObject(MovableGameObject *mGameObject) {
				mGameObject->registerHitCallback(std::bind(&ParticleEngine::gameObjectHit, this, mGameObject));
				mGameObject->registerStateChangedCallback(std::bind(&ParticleEngine::gameObjectStateChange, this, mGameObject));
			}

			bool ParticleEngine::hasNextParticleInstance() {
				if (!_drawContainer.empty()) {
					while (_drawContainer.size() > allowed_instances) {
						ParticleInstance *i = _drawContainer.front();
						delete i;
						_drawContainer.erase(_drawContainer.begin());
					}
				}
				return !_drawContainer.empty();
			}

			std::vector<ParticleInstance*> ParticleEngine::getNextParticleInstance() {
				if (!_drawContainer.empty()) {
					return _drawContainer;
				} 
			}

			void ParticleEngine::unloadAll() {
				if (_particleSets.size() > 0) {
					std::map<std::string, ParticleSet*>::iterator itr = _particleSets.begin();
					while (itr != _particleSets.end()) {
						delete itr->second;
						_particleSets.erase(itr++);
					}
				}

				if (_drawContainer.size() > 0) {
					std::vector<ParticleInstance*>::iterator it;
					for (it = _drawContainer.begin(); it != _drawContainer.end();) {
						delete *it;
						it = _drawContainer.erase(it);
					}
				}
			}
		}
	}
}
