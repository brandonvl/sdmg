#pragma once
#include <SDL.h>

#include <vector>

namespace sdmg {
	namespace engine {
		namespace particle {
			class Particle;
			class ParticleSet
			{
			public:
				ParticleSet(int max, int x, int y, int xVel, int yVel, int width, int height, SDL_Surface *image);
				ParticleSet(const ParticleSet& other, int xVel, int yVel);
				virtual ~ParticleSet();
				
				void reset();
				void show();
				void showContinuous();
				void resetLifeTime();
				bool isDead();
				void initParticles(int max, int x, int y, int xVel, int yVel, int width, int height, SDL_Surface *image);
				SDL_Surface* getSDLSurface();
				Particle* createParticle();
				int getXVel();
				int getYVel();
			private:
				int _x, _y, _max, _width, _height, _xVel, _yVel;
				SDL_Surface* _surface = nullptr;
				SDL_Surface* _image = nullptr;
				std::vector<Particle*> _particles;
			};
		}
	}
}