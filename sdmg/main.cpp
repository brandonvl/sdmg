#include <iostream>
#include "sdl\include\SDL.h"
#include "engine\physics\PhysicsEngine.h"
#include "model\Character.h"
#include "model\Platform.h"
#include "sdl\include\SDL.h"

using namespace sdmg::engine::physics;
using namespace sdmg::model;

SDL_Window *_window;
SDL_Renderer *_renderer;
bool running;
PhysicsEngine *_engine;
const int WIDTH = 1280, HEIGHT = 720;

void initSDL()
{
	int SDL_Init(SDL_INIT_EVERYTHING);
	_window = SDL_CreateWindow("Six Developers Make a Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	// Setup renderer
	_renderer = SDL_CreateRenderer(_window, 0, SDL_RENDERER_ACCELERATED);

	// _boxSizes = new std::map<b2Body*, b2Vec2*>;
}

void renderBodies()
{
	const float M2P = 20.0f;
	const float P2M = 1.0f / M2P;
	b2Body *body = _engine->getBodyList();
	b2Vec2 leftUpperPoint;

	while (body)
	{
		SDL_Rect r;
		GameObject *object = static_cast<GameObject*>(body->GetUserData());

		leftUpperPoint = ((b2PolygonShape*)body->GetFixtureList()->GetShape())->GetVertex(0);
		leftUpperPoint += body->GetWorldCenter();

		r.x = leftUpperPoint.x * M2P;
		r.y = leftUpperPoint.y * M2P;
		r.w = object->getWidth();
		r.h = object->getHeight();
		
		// Render rect
		SDL_RenderFillRect(_renderer, &r);

		//drawSquare(points, tmp->GetWorldCenter(), tmp->GetAngle());
		body = body->GetNext();
	}
}

void loadLevel1()
{
	Platform *ground = new Platform();
	ground->setSize(WIDTH, 30);
	Platform *patform = new Platform();
	patform->setSize(WIDTH / 2, 30);

	_engine->addBody(WIDTH / 2, HEIGHT - 50, ground->getWidth(), ground->getHeight(), false, ground);

	_engine->addBody(WIDTH / 4, HEIGHT - 200, ground->getWidth() / 2, ground->getHeight(), false, patform);
}

int main(int argc, char **argv)
{
	_engine = new PhysicsEngine();
	/*
	Character *char2 = new Character();

	_engine->doAction(char2, PhysicsEngine::Action::MOVELEFT);
	_engine->doAction(char2, PhysicsEngine::Action::MOVERIGHT);
	_engine->doAction(char2, PhysicsEngine::Action::IDLE);
	_engine->doAction(char2, PhysicsEngine::Action::JUMP);
	*/
	_engine->setWorldGravity(0.0f, 100.0f);
	loadLevel1();

	Character *player = new Character();
	player->setSize(50, 50);
	player->setSpeed(20, _engine->getWorldGravity().y / 15);

	_engine->addBody(50, 50, player->getWidth(), player->getHeight(), true, player);

	_engine->resume();

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
		//  handleInput();

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
					_engine->doAction(player, PhysicsEngine::Action::MOVERIGHT);
					break;
				case SDLK_LEFT:
					_engine->doAction(player, PhysicsEngine::Action::MOVELEFT);
					break;
				case SDLK_SPACE:
					_engine->doAction(player, PhysicsEngine::Action::JUMP);
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					_engine->doAction(player, PhysicsEngine::Action::IDLE);
					break;
				case SDLK_RIGHT:
					_engine->doAction(player, PhysicsEngine::Action::IDLE);
					break;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				//  _world->addBody(event.button.x, event.button.y, 20, 20, true);
				break;
			default:
				//  _player->move(0);
				break;
			}
		}

		SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255);

		_engine->update();
		renderBodies();
		SDL_RenderPresent(_renderer);

		
		if (1000.0 / 30 > SDL_GetTicks() - start)
			SDL_Delay(1000.0 / 30 - (SDL_GetTicks() - start));
			
	}
	SDL_Quit();

	return EXIT_SUCCESS;
	//  std::cin.get();
	//  return 0;
}


