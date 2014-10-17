//
//
//  @ Project : SDMG
//  @ File Name : SelectionState.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "SelectionState.h"
#include "engine\GameTime.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"

namespace sdmg {
	namespace gamestates {
		void SelectionState::init(GameBase &game)
		{
		}

		void SelectionState::cleanup(GameBase &game)
		{
		}

		void SelectionState::pause(GameBase &game)
		{
		}

		void SelectionState::resume(GameBase &game)
		{
		}

		void SelectionState::handleEvents(GameBase &game, GameTime &gameTime)
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

		void SelectionState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void SelectionState::draw(GameBase &game, GameTime &gameTime)
		{
		}
		
	}
}
