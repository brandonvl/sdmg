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

//#include "vld\include\vld.h"

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);
	Game *game = new Game();

	game->start();
	delete game;
	SDL_Quit();
	return 0;
}
