//
//
//  @ Project : SDMG
//  @ File Name : SelectionState.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "MenuState.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace gamestates {
		class SelectionState : public MenuState {
		public:
			virtual void init(GameBase &game);
			virtual void cleanup(GameBase &game);

			virtual void pause(GameBase &game);
			virtual void resume(GameBase &game);

			virtual void handleEvents(GameBase &game, GameTime &gameTime);
			virtual void update(GameBase &game, GameTime &gameTime);
			virtual void draw(GameBase &game, GameTime &gameTime);
			static SelectionState& getInstance() {
				static SelectionState _instance;
				return _instance;
			}
		protected:
			SelectionState() { }
		private:
		};
	}
}