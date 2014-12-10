#include "ParticleSet.h"
#include "Particle.h"

namespace sdmg {
	namespace engine {
		namespace particle {
			ParticleSet::ParticleSet(int max, int x, int y, int xVel, int yVel, int width, int height, SDL_Surface *image) {
				initParticles(max, x, y, xVel, yVel, width, height, image);
			}

			ParticleSet::ParticleSet(const ParticleSet& other, int xVel, int yVel) {
				initParticles(other._max, other._x, other._y, xVel, yVel, other._width, other._height, other._image);
			}

			ParticleSet::~ParticleSet() {
				std::vector<Particle*>::iterator it;
				for (it = _particles.begin(); it != _particles.end();) {
					delete *it;
					it = _particles.erase(it);
				}
			}

			void ParticleSet::initParticles(int max, int x, int y, int xVel, int yVel, int width, int height, SDL_Surface *image) {
				_height = height;
				_width = width;
				_x = x;
				_y = y;
				_xVel = xVel;
				_yVel = yVel;
				_max = max;
				_image = image;

				_surface = SDL_CreateRGBSurface(0, width, height, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

				// Create particles
				for (int i = 0; i < _max; i++)
					_particles.push_back(createParticle());
			}

			Particle* ParticleSet::createParticle() {
				return new Particle(_x + rand() % 6 - 3, // Particle x position
					_y + rand() % 6 - 3, // Paritcle y position
					rand() % 10 + (float)rand() / (float)RAND_MAX - _xVel, // Particle x velocity
					rand() % 10 + (float)rand() / (float)RAND_MAX - _yVel, // Particle y velocity
					5 + rand() % 200, // Particle lifetime
					rand() % 255, // Particle color
					_surface); // Surface to draw on
			}

			void ParticleSet::showContinuous() {
				for (int i = 0; i < _max; i++)
				{
					if (_particles[i]->isDead()) {
						delete _particles[i];
						_particles[i] = createParticle();
					}
					else {
						_particles[i]->move();
						_particles[i]->showByImage(_image);
					}
				}
			}

			void ParticleSet::reset() {
				for (int i = 0; i < _particles.size(); i++)
				{
					if (_particles[i]->isDead()) {
						delete _particles[i];
						_particles[i] = createParticle();
					}
				}
			}

			void ParticleSet::show() {
				for (int i = 0; i < _particles.size(); i++)
				{
					if (!_particles[i]->isDead()) {
						_particles[i]->move();
						_particles[i]->showByImage(_image);
					}
				}
			}

			bool ParticleSet::isDead() {
				bool allDead = true;
				for (int i = 0; i < _particles.size(); i++)
				{
					if (!_particles[i]->isDead()) {
						allDead = false;
					}
				}
				return allDead;
			}

			void ParticleSet::resetLifeTime() {
				if (_particles.size() > 0) {
					for (int i = 0; i < _particles.size(); i++)
					{
						_particles[i]->resetLife();
					}
				}
			}

			SDL_Surface* ParticleSet::getSDLSurface() {
				return _surface;
			}

			int ParticleSet::getXVel() {
				return _xVel;
			}

			int ParticleSet::getYVel() {
				return _yVel;
			}
		}
	}
}
