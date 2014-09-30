#include <iostream>
#include "sdl\include\SDL.h"
#include "Game.h"


#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include <string>

using namespace sdmg;

int main(int argc, char **argv)
{
	Game *game = new Game();

	Engine *engine = game->getEngine();
	DrawEngine *draw = engine->getDrawEngine();
	draw->load("chicken", "C:/Users/Niek Willems/Documents/GitHub/ChickenShooter/ChickenShooter/Images/chicken.png");

	game->start();
	SDL_Quit();

	return 0;
}