//
//
//  @ Project : SDMG
//  @ File Name : LoadingState.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "LoadingState.h"
#include "IntroState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"

namespace sdmg {
	namespace gamestates {

		void LoadingState::init(GameBase &game)
		{
			game.getEngine()->getDrawEngine()->load("cowboy", R"(assets\cowboy.jpg)");
			std::cout << "Initing LoadingState ... " << std::endl;
		}

		void LoadingState::cleanup(GameBase &game)
		{
			game.getEngine()->getDrawEngine()->unload("cowboy");
			std::cout << "Cleaning up LoadingState ... " << std::endl;
		}

		void LoadingState::pause(GameBase &game)
		{
			std::cout << "Pausing LoadingState ... " << std::endl;
		}

		void LoadingState::resume(GameBase &game)
		{
			std::cout << "Resuming LoadingState ... " << std::endl;
		}

		void LoadingState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			//std::cout << "Handling events LoadingState ... " << std::endl;
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
						changeState(game, IntroState::getInstance());
						break;
					}
				}
			}
		}

		void LoadingState::update(GameBase &game, GameTime &gameTime)
		{
			//std::cout << "Updating LoadingState ... " << std::endl;
		}

		void LoadingState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("cowboy", 0, 0);
			game.getEngine()->getDrawEngine()->render();
			//std::cout << "Draw LoadingState ... " << std::endl;
		}
		
	}
}
