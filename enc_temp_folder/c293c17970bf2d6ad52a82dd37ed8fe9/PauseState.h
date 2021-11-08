//
//
//  @ Project : SDMG
//  @ File Name : PauseState.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "MenuState.h"
#include <string>
#include <vector>

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
		class PauseState : public MenuState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);
			static PauseState& getInstance() {
				static PauseState _instance;
				return _instance;
			}
		protected:
			PauseState() { }
		private:
			void returnToMainMenu();
			void returnToLevelSelection();
			GameBase *_game;
		};
	}
}