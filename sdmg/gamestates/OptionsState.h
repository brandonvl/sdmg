//  @ File Name : MainMenuState.h
//  @ Date : 25-10-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "MenuState.h"

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
		class OptionsState : public MenuState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void pause(GameBase &game);
			void resume(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);
			static OptionsState& getInstance() {
				static OptionsState _instance;
				return _instance;
			}

		protected:
			OptionsState() { }
			void menuAction(MenuItem *item) override;

		private:
			GameBase *_game;
		};
	}
}