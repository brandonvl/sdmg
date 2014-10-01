#include <iostream>
#include "sdl\include\SDL.h"
#include "Game.h"


#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include <string>

using namespace sdmg;

#include "engine\GameBase.h"
#include "engine\GameStateManager.h"
#include "engine\GameState.h"
#include "gamestates\IntroState.h"

int main(int argc, char **argv)
{
	std::cin.get();
	Game *game = new Game();

	Engine *engine = game->getEngine();
	DrawEngine *draw = engine->getDrawEngine();
	draw->load("chicken", "c:/data/cow.png");

	game->start();
	SDL_Quit();
	return 0;
}