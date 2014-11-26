//
//
//  @ Project : SDMG
//  @ File Name : ParticleEngine.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#include "ParticleEngine.h"
#include "Particle.h"
#include "..\..\sdl\include\SDL.h"
#include "..\..\sdl\include\SDL_image.h"

#include "engine\drawing\Rectangle.h"

#include <ctime>

namespace sdmg {
	namespace engine {
		namespace particle {
			ParticleEngine::ParticleEngine() {
				// Seed random generator
				srand(time(0));

				// Load particle images
				loadParticles();

				_particles = std::vector<Particle*>();
			}

			void ParticleEngine::loadParticles() {
				int pngFlags = IMG_INIT_PNG;
				if ((IMG_Init(pngFlags) & pngFlags)) {
					_ptclRed = IMG_Load("assets/particles/red.png");
					_ptclYellow = IMG_Load("assets/particles/yellow.png");
					SDL_SetSurfaceAlphaMod(_ptclRed, SDL_ALPHA_OPAQUE);
					SDL_SetSurfaceAlphaMod(_ptclYellow, SDL_ALPHA_OPAQUE);
				}
			}

			ParticleEngine::~ParticleEngine() {
				// Delete all particles
				for (int i = 0; i< _max; i++)
					delete _particles[i];
			}

			void ParticleEngine::setParticles(int max, int x, int y, int width, int height) {
				_surface = SDL_CreateRGBSurface(0, width, height, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

				_x = x;
				_y = y;
				_max = max;

				// Create particles
				for (int i = 0; i < _max; i++)
					_particles.push_back(createParticle());
			}

			Particle* ParticleEngine::createParticle() {
				return new Particle(_x + rand() % 6 - 3, // Particle x position
									_y + rand() % 6 - 3, // Paritcle y position
									rand() % 10 + (float)rand() / (float)RAND_MAX - 5, // Particle x velocity
									rand() % 10 + (float)rand() / (float)RAND_MAX - 5, // Particle y velocity
									500 + rand() % 1000, rand() % 255, // Particle color
									_surface); // Surface to draw on
			}

			void ParticleEngine::refresh() {
				for (int i = 0; i < _max; i++)
				{
					if (_particles[i]->isDead()) {
						delete _particles[i];
						_particles[i] = createParticle();
					}
					else {
						_particles[i]->move();
						_particles[i]->showByImage(_ptclRed);
					}
				} 
			}

			SDL_Surface* ParticleEngine::getSDLSurface() {
				return _surface;
			}
		}
	}
}
