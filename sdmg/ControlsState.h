//
//
//  @ Project : SDMG
//  @ File Name : ControlsState.h
//  @ Date : 21109-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
// #include "engine\GameState.h"
#include "gamestates\MenuState.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace engine {
		class GameTime;
	}
	namespace helperclasses {
		class Menu;
		class MenuItem;
	}

	namespace gamestates {
		using namespace sdmg::helperclasses;
		class ControlsState : public MenuState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void pause(GameBase &game);
			void resume(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);
			static ControlsState& getInstance() {
				static ControlsState _instance;
				return _instance;
			}

		protected:
			ControlsState() { }
			void menuAction(MenuItem *item) override;
			void bindKey(const int keyCode);
		private:
			GameBase *_game;
			bool waiting = false;
			int currentplayer = 1;
		};
	}
}