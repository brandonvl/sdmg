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
#include <vector>
#include "..\..\sdl\include\SDL.h"

namespace sdmg {
	namespace engine {
		class Engine;
		class GameObject;
		class MovableGameObject;
		namespace particle {
			class ParticleSet;
			class ParticleInstance;
			class ParticleEngine
			{
			public:
				ParticleEngine();
				virtual ~ParticleEngine();

				void createParticleSet(std::string key, int max, int x, int y, int width, int height, std::string image);
				void continuousShowParticleSet(std::string key);
				void continuousShowParticleSet(ParticleInstance* instance);
				void resetParticleLifeTime(std::string key);
				void resetParticleLifeTime(ParticleInstance* instance);
				void resetParticleSet(std::string key);
				void resetParticleSet(ParticleInstance* instance);
				void showParticleSet(ParticleInstance* instance);
				SDL_Surface* getParticleSetSurface(ParticleInstance* instance);
				SDL_Surface* ParticleEngine::getParticleSetSurface(std::string key);
				void gameObjectHit(MovableGameObject *gameObject);
				void registerGameObject(MovableGameObject *mGameObject);
				std::vector<ParticleInstance*> getNextParticleInstance();
				bool hasNextParticleInstance();
			private:
				std::map<std::string, ParticleSet*> _particleSets;
				std::map<std::string, SDL_Surface*> _particleImages;
				std::vector<ParticleInstance*> _drawContainer;

				void loadParticles();
			};
		}
	}
}

