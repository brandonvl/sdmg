//
//
//  @ Project : SDMG
//  @ File Name : IntroState.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#include "IntroState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "LoadingState.h"

namespace sdmg {
	namespace gamestates {

		void IntroState::init(GameBase &game)
		{
			game.getEngine()->getDrawEngine()->load("surprise", R"(assets\surprise.png)");
			game.getEngine()->getDrawEngine()->loadMap("explosion", R"(assets\explosion.png)", 64, 64);
			game.getEngine()->getDrawEngine()->loadMap("nivek", R"(assets\nivek.png)", 440.25, 573);
			std::cout << "Initing IntroState ... " << std::endl;
		}

		void IntroState::cleanup(GameBase &game)
		{
			std::cout << "Cleaning up IntroState ... " << std::endl;
			game.getEngine()->getDrawEngine()->unload("surprise");
		}

		void IntroState::pause(GameBase &game)
		{
			std::cout << "Pausing IntroState ... " << std::endl;
		}

		void IntroState::resume(GameBase &game)
		{
			std::cout << "Resuming IntroState ... " << std::endl;
		}

		void IntroState::handleEvents(GameBase &game, GameTime &gameTime)
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
						changeState(game, LoadingState::getInstance());
						break;
					}
				}
			}
		}

		void IntroState::update(GameBase &game, GameTime &gameTime)
		{
			//std::cout << "Updating IntroState ... " << std::endl;
		}

		void IntroState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("surprise", Rectangle(0, 0, 266, 330));
			game.getEngine()->getDrawEngine()->draw("nivek", Rectangle(280, 10, 219, 286.5), gameTime.getTotalMilisecondsRunning() / 100);

			// simple sprite animation
			game.getEngine()->getDrawEngine()->draw("explosion", Rectangle(200, 150, 64, 64), gameTime.getTotalMilisecondsRunning() / 100);
			game.getEngine()->getDrawEngine()->render();
			//std::cout << "Draw IntroState ... " << std::endl;
		}
		
	}
}
