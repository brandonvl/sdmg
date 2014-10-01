//
//
//  @ Project : SDMG
//  @ File Name : LevelSelectionState.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "SelectionState.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace engine {
		class GameTime;
	}
	
	namespace gamestates {
		class LevelSelectionState : public SelectionState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void pause(GameBase &game);
			void resume(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);
			static LevelSelectionState& getInstance() {
				static LevelSelectionState _instance;
				return _instance;
			}
		protected:
			LevelSelectionState() { }
		private:
		};
	}
}