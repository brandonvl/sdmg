//
//
//  @ Project : SDMG
//  @ File Name : MainMenuState.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "MainMenuState.h"
#include "engine\GameTime.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"

namespace sdmg {
	namespace gamestates {
		void MainMenuState::init(GameBase &game)
		{
			game.getEngine()->getDrawEngine()->load("surprise", R"(assets\surprise.png)");
			std::cout << "Initing IntroState ... " << std::endl;
		}

		void MainMenuState::cleanup(GameBase &game)
		{
			std::cout << "Cleaning up IntroState ... " << std::endl;
			game.getEngine()->getDrawEngine()->unload("surprise");
		}

		void MainMenuState::pause(GameBase &game)
		{
			std::cout << "Pausing IntroState ... " << std::endl;
		}

		void MainMenuState::resume(GameBase &game)
		{
			std::cout << "Resuming IntroState ... " << std::endl;
		}

		void MainMenuState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			//std::cout << "Handling events IntroState ... " << std::endl;
			SDL_Event event;
			if (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					game.stop();
				}

				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						game.stop();
						break;
					case SDLK_1:
						std::cout << "Key 1 pressed. Switching State.. " << std::endl;
						//changeState(game, LoadingState::getInstance());
						break;
					}
				}
			}
		}

		void MainMenuState::update(GameBase &game, GameTime &gameTime)
		{
			//std::cout << "Updating IntroState ... " << std::endl;
		}

		void MainMenuState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->draw("surprise", 0, 0);
			//std::cout << "Draw IntroState ... " << std::endl;
		}

		
	}
}
