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
#include "MainMenuState.h"

namespace sdmg {
	namespace gamestates {

		void IntroState::init(GameBase &game)
		{
			game.getEngine()->getDrawEngine()->load("loading", "assets\\screens\\loadingscreen");
			_fullscreen = false;
		}

		void IntroState::cleanup(GameBase &game)
		{
			game.getEngine()->getDrawEngine()->unload("loading");
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
					}
				}
			}
		}

		void IntroState::update(GameBase &game, GameTime &gameTime)
		{
			if (gameTime.getTotalSecondsRunning() > 1 && !_fullscreen) {
				game.getEngine()->getDrawEngine()->setFullscreen(true);
				_fullscreen = true;
			}
			else if (gameTime.getTotalSecondsRunning() > 5) {
				changeState(game, MainMenuState::getInstance());
			}

			//std::cout << "Updating IntroState ... " << std::endl;
		}

		void IntroState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("loading");
			game.getEngine()->getDrawEngine()->render();
		}
		
	}
}
