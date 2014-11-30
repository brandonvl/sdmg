#pragma once
#include "..\..\sdl\include\SDL.h"

#include <vector>

namespace sdmg {
	namespace engine {
		namespace particle {
			class Particle;
			class ParticleSet
			{
			public:
				ParticleSet(int max, int x, int y, int width, int height, SDL_Surface *image);
				virtual ~ParticleSet();
				
				void reset();
				void show();
				void showContinuous();
				void resetLifeTime();
				void initParticles(int max, int x, int y, int width, int height, SDL_Surface *image);
				SDL_Surface* getSDLSurface();
				Particle* createParticle();
			private:
				int _x, _y, _max;
				SDL_Surface* _surface;
				SDL_Surface* _image;
				std::vector<Particle*> _particles;
			};
		}
	}
}

