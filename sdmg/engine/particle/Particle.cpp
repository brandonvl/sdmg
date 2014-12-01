//
//
//  @ Project : SDMG
//  @ File Name : Particle.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#include "Particle.h"
#include "sdl\include\SDL_image.h"

namespace sdmg {
	namespace engine {
		namespace particle {
			Particle::Particle(float x, float y, float xVel, float yVel, int life, int color, SDL_Surface *surface) {
				_x = x;
				_y = y;
				_xVel = xVel;
				_yVel = yVel;
				_life = life;
				_endTime = SDL_GetTicks() + life; 
				_color = color;
				_surface = surface;
			}

			Particle::~Particle() {
			}

			void Particle::resetLife() {
				_endTime = SDL_GetTicks() + _life;
			}

			void Particle::move() {
				// Move
				_x += _xVel;
				_y += _yVel;
				// Calculate boundaries
				if (_x < 0) _x = 0;
				if (_y < 0) _y = 0;
				if (_x > _surface->w) _x = _surface->w - 1;
				if (_y > _surface->h) _y = _surface->h - 1;
			}

			void Particle::showByImage(SDL_Surface *image) {
				SDL_Rect rect;
				rect.h = image->h;
				rect.w = image->w;
				rect.x = _x;
				rect.y = _y;

				SDL_BlitSurface(image, NULL, _surface, &rect);
			}

			void Particle::showPixel() {
				Uint8* pixels = (Uint8*)_surface->pixels;
				Uint8* pixel = pixels + (int)_y * _surface->pitch + (int)_x;
				*((Uint32*)pixel) = (Uint32)_color;
			}

			bool Particle::isDying() {
				return (SDL_GetTicks() >= (_endTime - (_life / 2)));
			}

			bool Particle::isDead() {
				return (SDL_GetTicks() >= _endTime || _x == 0 || _y == 0 || 
					_x == _surface->w - 1 || _y == _surface->h - 1);
			}
		}
	}
}
