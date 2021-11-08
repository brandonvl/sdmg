//
//
//  @ Project : SDMG
//  @ File Name : GameOverSurvivalState.h
//  @ Date : 10-12-2014
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
		class GameOverSurvivalState : public MenuState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);
			static GameOverSurvivalState& getInstance() {
				static GameOverSurvivalState _instance;
				return _instance;
			}

		protected:
			GameOverSurvivalState() { }
		private:
			GameBase *_game = nullptr;

			bool _replay, _isLoaded;
			long _enteredGameOverState;
			
			void replay();
		};
	}
}