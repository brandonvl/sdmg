//  @ File Name : ProgressLoadState.h
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
		class ProgressLoadState : public MenuState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);
			static ProgressLoadState& getInstance() {
				static ProgressLoadState _instance;
				return _instance;
			}

		protected:
			ProgressLoadState() { }

		private:
			GameBase *_game = nullptr;
			void loadText(std::string key, std::string text, std::string fontName, int fontSize);
		};
	}
}