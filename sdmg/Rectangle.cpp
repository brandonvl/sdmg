#include <iostream>
#include "sdl\include\SDL.h"
#include "Box2D\Box2D.h"
#include <map>

const int WIDTH = 1280;
const int HEIGHT = 720;
float M2P, P2M;
b2World *world;
SDL_Window *window;
SDL_Renderer *renderer;
std::map<b2Body*, b2Vec2> *_boxSizes;

void initSDL()
{
	int SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Six Developers Make a Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	// Setup renderer
	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

	_boxSizes = new std::map<b2Body*, b2Vec2>;
}


b2Body* addBody(int x, int y, int w, int h, bool dyn = true)
{
	b2BodyDef bodydef;
	bodydef.position.Set(x*P2M, y*P2M);
	if (dyn)
		bodydef.type = b2_dynamicBody;
	b2Body* body = world->CreateBody(&bodydef);

	b2PolygonShape shape;
	shape.SetAsBox(P2M*w / 2, P2M*h / 2);

	b2FixtureDef fixturedef;
	fixturedef.shape = &shape;
	fixturedef.density = 1.0;
	body->CreateFixture(&fixturedef);

	b2Vec2 vec;
	vec.Set(P2M*w, P2M*h);
	_boxSizes->insert(std::pair<b2Body*, b2Vec2>(body, vec));

	return body;
}

void initWorld()
{
	M2P = 20.0f;
	P2M = 1.0f / M2P;
	world = new b2World(b2Vec2(0.0, 9.81));

	addBody(WIDTH / 2, HEIGHT - 50, WIDTH, 30, false);
}

void rotateTranslate(b2Vec2& vector, const b2Vec2& center, float angle)
{
	b2Vec2 tmp;
	tmp.x = vector.x*cos(angle) - vector.y*sin(angle);
	tmp.y = vector.x*sin(angle) + vector.y*cos(angle);
	vector = tmp + center;
}

void display()
{
	//SDL_FillRect(screen, NULL, 0);
	b2Body* tmp = world->GetBodyList();
	b2Vec2 points[4];

	while (tmp)
	{
		for (int i = 0; i<4; i++)
		{
			points[i] = ((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->GetVertex(i);
			rotateTranslate(points[i], tmp->GetWorldCenter(), tmp->GetAngle());
		}
		SDL_Rect r;
		r.x = points[0].x * M2P;
		r.y = points[0].y * M2P;
		b2Vec2 vec = (*_boxSizes)[tmp];
		r.w = vec.x * M2P;
		r.h = vec.y * M2P;

		// Render rect
		SDL_RenderFillRect(renderer, &r);

		//drawSquare(points, tmp->GetWorldCenter(), tmp->GetAngle());
		tmp = tmp->GetNext();
	}
}

int main(int argc, char **argv)
{
	initSDL();

	float32 time = 60.0f;
	float32 timeStep = 1.0f / time;

	Uint32 start;
	SDL_Event event;
	bool running = true;
	initWorld();
	while (running)
	{
		// Set render color to red ( background will be rendered in this color )
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		// Clear winow
		SDL_RenderClear(renderer);

		start = SDL_GetTicks();
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
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				addBody(event.button.x, event.button.y, 20, 20, true);
				break;
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		display();

		//  for (int i = 0; i < 30; i ++)
			world->Step(timeStep, 8, 3);

		SDL_RenderPresent(renderer);

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