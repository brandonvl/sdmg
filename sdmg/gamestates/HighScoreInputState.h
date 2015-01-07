#pragma once
#include "MenuState.h"
#include <vector>

using namespace sdmg::engine;

namespace JSON { class JSONDocument; }
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
		class HighScoreInputState : public MenuState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);

			static HighScoreInputState& getInstance() {
				static HighScoreInputState _instance;
				return _instance;
			}

			void setHighscore(int score) { _highscore = score; }
		
		protected:
			HighScoreInputState() { }

		private:
			GameBase *_game;
			Menu *_menu;
			int _highscore;

			void loadText(std::string key, std::string text, std::string fontName, int fontSize);

			std::string *_highscoreInitials;
		};
	}
}