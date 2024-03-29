#include <iostream>
#include <SDL.h>
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
#include "helperclasses\ConfigManager.h"
#include "engine/util/FileManager.h"

int main(int argc, char **argv)
{
	engine::util::FileManager::getInstance().setExecutablePath(argv[0]);
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);
	Game *game = new Game();
	game->start();
	delete game;
	SDL_Quit();
	return 0;
}
