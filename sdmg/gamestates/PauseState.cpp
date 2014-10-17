//
//
//  @ Project : SDMG
//  @ File Name : PauseState.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "PauseState.h"
#include "engine\GameTime.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"

namespace sdmg {
	namespace gamestates {
		void PauseState::init(GameBase &game)
		{
		}

		void PauseState::cleanup(GameBase &game)
		{
		}

		void PauseState::pause(GameBase &game)
		{
		}

		void PauseState::resume(GameBase &game)
		{
		}

		void PauseState::handleEvents(GameBase &game, GameTime &gameTime)
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

		void PauseState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void PauseState::draw(GameBase &game, GameTime &gameTime)
		{
		}		
	}
}
