//
//
//  @ Project : SDMG
//  @ File Name : MenuState.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "MenuState.h"
#include "engine\GameTime.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"

namespace sdmg {
	namespace gamestates {
		void MenuState::init(GameBase &game)
		{
		}

		void MenuState::cleanup(GameBase &game)
		{
		}

		void MenuState::pause(GameBase &game)
		{
		}

		void MenuState::resume(GameBase &game)
		{
		}

		void MenuState::handleEvents(GameBase &game, GameTime &gameTime)
		{
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

		void MenuState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void MenuState::draw(GameBase &game, GameTime &gameTime)
		{
		}		
	}
}
