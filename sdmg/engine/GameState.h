//
//
//  @ Project : SDMG
//  @ File Name : GameState.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once

#include "GameBase.h"
#include "GameStateManager.h"

namespace sdmg {
	namespace engine {
		class GameTime;

		class GameState {
		public:
			virtual void init() = 0;
			virtual void cleanup() = 0;

			virtual void pause() = 0;
			virtual void resume() = 0;

			virtual void handleEvents(GameBase *game) = 0;
			virtual void update(GameBase *game) = 0;
			virtual void draw(GameBase *game) = 0;

			void changeState(GameBase *game, GameState *state) {
				game->getStateManager()->changeState(state);
			}

		protected:
			GameState() { }
		};
	}
}