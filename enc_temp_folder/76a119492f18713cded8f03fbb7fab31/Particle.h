//
//
//  @ Project : SDMG
//  @ File Name : Particle.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include <SDL.h>

namespace sdmg {
	namespace engine {
		namespace particle {
			class Particle
			{
			public:
				Particle(float x, float y, float xVel, float yVel, int life, int color, SDL_Surface *surface);
				virtual ~Particle();

				void move();
				void showByImage(SDL_Surface *image);
				void showPixel();
				bool isDead();
				bool isDying();
				void resetLife();

			private:
				SDL_Surface *_surface;
				//SDL_Surface *_image;
				float _x, _y, _xVel, _yVel;
				int _endTime;
				int _life;
				int _color;
			};
		}
	}
}

