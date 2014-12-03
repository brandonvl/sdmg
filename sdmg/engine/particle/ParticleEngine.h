//
//
//  @ Project : SDMG
//  @ File Name : ParticleEngine.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include <map>
#include "..\..\sdl\include\SDL.h"

namespace sdmg {
	namespace engine {
		class Engine;
		class GameObject;
		class MovableGameObject;
		namespace particle {
			class ParticleSet;
			class ParticleEngine
			{
			public:
				ParticleEngine();
				virtual ~ParticleEngine();

				void createParticleSet(std::string key, int max, int x, int y, int width, int height);
				void continuousShowParticleSet(std::string key);
				void resetParticleLifeTime(std::string key);
				void resetParticleSet(std::string key);
				void showParticleSet(std::string key);
				SDL_Surface* getParticleSetSurface(std::string key);
				void gameObjectHit(MovableGameObject *gameObject);
				void registerGameObject(MovableGameObject *mGameObject);

				void unloadAll();

				int getX();
				int getY();
				void resetXY();
			private:
				std::map<std::string, ParticleSet*> _particleSets;

				void loadParticles();

				// Particle images
				SDL_Surface* _ptclRed;
				SDL_Surface* _ptclYellow;

				int _x, _y;
			};
		}
	}
}

