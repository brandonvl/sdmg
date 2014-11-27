//
//
//  @ Project : SDMG
//  @ File Name : ParticleEngine.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include <vector>
#include "..\..\sdl\include\SDL.h"

namespace sdmg {
	namespace engine {
		class Engine;
		namespace particle {
			class Particle;
			class ParticleEngine
			{
			public:
				ParticleEngine();
				virtual ~ParticleEngine();

				void refresh();
				void setParticles(int max, int x, int y, int width, int height);
				void loadParticles();
				SDL_Surface* getSDLSurface();
				Particle* createParticle();
			private:
				int _x, _y, _max;
				SDL_Surface* _surface;
				std::vector<Particle*> _particles;

				// Particle images
				SDL_Surface* _ptclRed;
				SDL_Surface* _ptclYellow;

			};
		}
	}
}

