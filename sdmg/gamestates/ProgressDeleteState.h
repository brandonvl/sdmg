//  @ File Name : ProgressDeleteState.h
//  @ Date : 29-11-2014
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
		class ProgressDeleteState : public MenuState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);
			static ProgressDeleteState& getInstance() {
				static ProgressDeleteState _instance;
				return _instance;
			}

		protected:
			ProgressDeleteState() { }

		private:
			GameBase *_game = nullptr;
			void loadText(std::string key, std::string text, std::string fontName, int fontSize);
		};
	}
}