#include "Box2D\Box2D.h"
#include "sdl\include\SDL.h"
#include "Level1.h"
#include "Player.h"
#include "World.h"
#include <map>
#include <iostream>

SDL_Window *_window;
SDL_Renderer *_renderer;
Player *_player;
World *_world;
bool running;

void initSDL()
{
	int SDL_Init(SDL_INIT_EVERYTHING);
	_window = SDL_CreateWindow("Six Developers Make a Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _world->getWidth(), _world->getHeight(), SDL_WINDOW_SHOWN);
	// Setup renderer
	_renderer = SDL_CreateRenderer(_window, 0, SDL_RENDERER_ACCELERATED);

	// _boxSizes = new std::map<b2Body*, b2Vec2*>;
}


void handleInput()
{
}

int main(int argc, char **argv)
{
	// _world = new World(b2Vec2(0.0, 9.81));
	_world = new World(b2Vec2(0.0, 25.0));

	Level1 *level1 = new Level1();
	level1->loadLevel(_world);

	// _world->addBody(_world->getWidth() / 2, _world->getHeight() - 50, _world->getWidth(), 30, false);

	_player = new Player(50, 50, 20, 3, _world->addBody(_world->getWidth() / 2, _world->getHeight() - 50, 50, 50, true));



	initSDL();

	float32 time = 60.0f;
	float32 timeStep = 1.0f / time;

	Uint32 start;
	SDL_Event event;
	running = true;
	while (running)
	{
		// Set render color to red ( background will be rendered in this color )
		SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
		// Clear winow
		SDL_RenderClear(_renderer);

		start = SDL_GetTicks();
		handleInput();



		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					running = false;
					break;
				case SDLK_RIGHT:
					_player->move(1);
					break;
				case SDLK_LEFT:
					_player->move(-1);
					break;
				case SDLK_SPACE:
					_player->jump();
					break;
				}
				break;
			case SDL_KEYUP:


				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					_player->move(0);
					break;
				case SDLK_RIGHT:
					_player->move(0);
					break;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				_world->addBody(event.button.x, event.button.y, 20, 20, true);
				break;
			default:
				// _player->move(0);
				break;
			}
		}


		SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255);

		_world->step(timeStep, 8, 3);

		_world->renderBodies(_renderer);

		SDL_RenderPresent(_renderer);

		if (1000.0 / 30 > SDL_GetTicks() - start)
			SDL_Delay(1000.0 / 30 - (SDL_GetTicks() - start));
	}
	SDL_Quit();

	return EXIT_SUCCESS;

	/*

	// Creat a rect at pos ( 50, 50 ) that's 50 pixels wide and 50 pixels high.
	SDL_Rect r;
	r.x = 50;
	r.y = 50;
	r.w = 50;
	r.h = 50;

	// Set render color to blue ( rect will be rendered in this color )
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

	// Render rect
	SDL_RenderFillRect(renderer, &r);

	// Render the rect to the screen
	SDL_RenderPresent(renderer);

	// Wait for 5 sec
	SDL_Delay(5000);
	SDL_DestroyWindow(window);
	SDL_Quit();
	*/
}