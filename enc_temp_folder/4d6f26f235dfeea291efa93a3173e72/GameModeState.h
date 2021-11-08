//
//
//  @ Project : SDMG
//  @ File Name : GameModeState.h
//  @ Date : 8-12-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "SelectionState.h"
#include <map>
#include <string>
#include <vector>
#include <functional>

using namespace sdmg::engine;

namespace sdmg {
	namespace engine {
		class GameTime;
	}

	namespace gamestates {
		class GameModeState : public SelectionState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);
			static GameModeState& getInstance() {
				static GameModeState _instance;
				return _instance;
			}
		protected:
			GameModeState() { }
		private:
			GameBase *_game;
		};
	}
}