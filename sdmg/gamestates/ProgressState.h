//
//
//  @ Project : SDMG
//  @ File Name : GameOverState.h
//  @ Date : 10-11-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "MenuState.h"

using namespace sdmg::engine;


namespace sdmg {
	namespace helperclasses {
		class Menu;
		class MenuItem;
	}

	namespace gamestates {
		class ProgressState :
			public MenuState
		{
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);

			static ProgressState& getInstance() {
				static ProgressState _instance;
				return _instance;
			}
			void setChanged();

		private:
			ProgressState() { }
			void loadText(std::string key, std::string text, std::string fontName, int fontSize);

			GameBase *_game;

			bool _isEnabled;
			bool _hasChanged;
		};
	}
}