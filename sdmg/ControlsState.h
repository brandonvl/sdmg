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

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);
			static ControlsState& getInstance() {
				static ControlsState _instance;
				return _instance;
			}

		protected:
			ControlsState() { }
			void save(std::string deviceName);
			void setKey(std::string keyCode, std::string deviceName);
			void waitFor(std::string *cur);
			bool checkKey(std::string keyCode);
			void readKeys();
			void setKeys();
		private:
			GameBase *_game;
			int _currentplayer;
			std::vector<std::string> *_keys;
			std::string *_info, *_info2, *_devicename, *_walkright, *_walkleft, *_jump, *_roll, *_midrange, *_longrange, *_block, *_current, *_oldkey;
		};
	}
}