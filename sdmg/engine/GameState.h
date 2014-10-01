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
#include "GameTime.h"
#include <iostream>

namespace sdmg {
	namespace engine {

		class GameState {
		public:
			virtual void init(GameBase &game) = 0;
			virtual void cleanup(GameBase &game) = 0;

			virtual void pause(GameBase &game) = 0;
			virtual void resume(GameBase &game) = 0;

			virtual void handleEvents(GameBase &game, GameTime &gameTime) = 0;
			virtual void update(GameBase &game, GameTime &gameTime) = 0;
			virtual void draw(GameBase &game, GameTime &gameTime) = 0;

			void changeState(GameBase &game, GameState &state) {
				game.getStateManager()->changeState(state);
			}

		protected:
			GameState() { }
		};
	}
}