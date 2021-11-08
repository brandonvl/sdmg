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
			GameBase *_game = nullptr;
			int _currentdevice;
			std::vector<std::string> *_keys = nullptr;
			std::vector<std::string> *_deviceKeys = nullptr;
			std::string *_info = nullptr, *_info2 = nullptr, *_devicename = nullptr, *_walkright = nullptr, *_walkleft = nullptr, *_jump = nullptr, *_roll = nullptr, *_midrange = nullptr, *_longrange = nullptr, *_block = nullptr, *_current = nullptr, *_oldkey = nullptr;
		};
	}
}