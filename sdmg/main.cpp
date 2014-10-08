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
#include "gamestates\LoadingState.h"

int main(int argc, char **argv)
{
	Game *game = new Game();

	game->start();
	SDL_Quit();
	return 0;
}
