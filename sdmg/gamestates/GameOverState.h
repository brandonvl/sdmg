//
//
//  @ Project : SDMG
//  @ File Name : GameOverState.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
// #include "engine\GameState.h"
#include "MenuState.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace engine {
		class GameTime;
	}
	namespace model {
		class Character;
	}
	namespace helperclasses {
		class Menu;
		class MenuItem;
	}

	namespace gamestates {
		using namespace sdmg::helperclasses;
		class GameOverState : public MenuState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);
			static GameOverState& getInstance() {
				static GameOverState _instance;
				return _instance;
			}

		protected:
			GameOverState() { }
		private:
			int _characterCount;
			GameBase *_game;
			bool _replay;

			void replay();
			void next();
		};
	}
}