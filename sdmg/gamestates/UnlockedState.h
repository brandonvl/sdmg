//
//
//  @ Project : SDMG
//  @ File Name : UnlockedState.h
//  @ Date : 9-12-2014
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
		class UnlockedState : public MenuState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);

			static UnlockedState& getInstance() {
				static UnlockedState _instance;
				return _instance;
			}

			void setPlayerName(std::string playerName);
			void setLevelName(std::string levelName);

		protected:
			UnlockedState() { }
		private:
			GameBase *_game = nullptr;

			std::string *_playerName = nullptr, *_levelName = nullptr;
		};
	}
}