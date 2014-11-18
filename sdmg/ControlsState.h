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
			void menuAction() override;
			void bindKey();
			void changeText(const int keyCode);
			void readKeys();
		private:
			GameBase *_game;
			bool _waiting;
			int _currentplayer;
			std::string* _info;
			std::string* _info2;
			std::string* _walkright;
			std::string* _walkleft;
			std::string* _jump;
			std::string* _roll;
			std::string* _midrange;
			std::string* _longrange;
			std::string* _block;
		};
	}
}