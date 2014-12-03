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
#include "..\..\sdl\include\SDL_image.h"
#include "engine\MovableGameObject.h"

#include <ctime>
#include <string>

namespace sdmg {
	namespace engine {
		namespace particle {
			ParticleEngine::ParticleEngine() {
				_particleSets = std::map<std::string, ParticleSet*>();

				// Seed random generator
				srand(time(0));

				// Load particle images
				loadParticles();
			}

			ParticleEngine::~ParticleEngine() {
				if (_particleSets.size() > 0) {
					std::map<std::string, ParticleSet*>::iterator itr = _particleSets.begin();
					while (itr != _particleSets.end()) {
						delete itr->second;
						_particleSets.erase(itr++);
					}
				}

				SDL_FreeSurface(_ptclRed);
				SDL_FreeSurface(_ptclYellow);
				
				_ptclRed = nullptr;
				_ptclYellow = nullptr;
			}

			void ParticleEngine::loadParticles() {
				int pngFlags = IMG_INIT_PNG;
				if ((IMG_Init(pngFlags) & pngFlags)) {
					_ptclRed = IMG_Load("assets/particles/blood.png");
					_ptclYellow = IMG_Load("assets/particles/yellow.png");
					SDL_SetSurfaceAlphaMod(_ptclRed, SDL_ALPHA_OPAQUE);
					SDL_SetSurfaceAlphaMod(_ptclYellow, SDL_ALPHA_OPAQUE);
				}
			}

			void ParticleEngine::createParticleSet(std::string key, int max, int x, int y, int width, int height) {
				ParticleSet *ps = new ParticleSet(max, x, y, width, height, _ptclRed);
				_particleSets.insert(std::make_pair(key, ps));
			}

			void  ParticleEngine::continuousShowParticleSet(std::string key) {
				_particleSets[key]->showContinuous();
			}

			void ParticleEngine::resetParticleLifeTime(std::string key) {
				_particleSets[key]->resetLifeTime();
			}

			void ParticleEngine::resetParticleSet(std::string key) {
				_particleSets[key]->reset();
			}

			void ParticleEngine::showParticleSet(std::string key) {
				_particleSets[key]->show();
			}

			SDL_Surface* ParticleEngine::getParticleSetSurface(std::string key) {
				return _particleSets[key]->getSDLSurface();
			}

			void ParticleEngine::gameObjectHit(MovableGameObject *gameObject) {
				std::string key = "hit";
				SDL_Surface* surface = _particleSets[key]->getSDLSurface();
				_x = (gameObject->getPixelX() - (surface->w / 2));
				_y = (gameObject->getPixelY() - (surface->h / 2));
				_particleSets[key]->reset();
			}

			void ParticleEngine::registerGameObject(MovableGameObject *mGameObject) {
				mGameObject->registerHitCallback(std::bind(&ParticleEngine::gameObjectHit, this, mGameObject));
			}

			int ParticleEngine::getX() { return _x; }
			int ParticleEngine::getY() { return _y; }
			void ParticleEngine::resetXY() {
				_x = 0;
				_y = 0;
			}

			void ParticleEngine::unloadAll()
			{
				if (_particleSets.size() > 0) {
					std::map<std::string, ParticleSet*>::iterator itr = _particleSets.begin();
					while (itr != _particleSets.end()) {
						delete
							itr->second;
						_particleSets.erase(itr++);
					}
				}

				/*
				delete _ptclRed;
				delete _ptclYellow;

				_ptclRed = nullptr;
				_ptclYellow = nullptr;
				*/
			}
		}
	}
}
