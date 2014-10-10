//
//
//  @ Project : SDMG
//  @ File Name : LevelSelectionState.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "LevelSelectionState.h"
#include "engine\GameTime.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"

namespace sdmg {
	namespace gamestates {
		void LevelSelectionState::init(GameBase &game)
		{
		}

		void LevelSelectionState::cleanup(GameBase &game)
		{
		}

		void LevelSelectionState::pause(GameBase &game)
		{
		}

		void LevelSelectionState::resume(GameBase &game)
		{
		}

		void LevelSelectionState::handleEvents(GameBase &game, GameTime &gameTime)
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

		void LevelSelectionState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void LevelSelectionState::draw(GameBase &game, GameTime &gameTime)
		{
		}		
	}
}
