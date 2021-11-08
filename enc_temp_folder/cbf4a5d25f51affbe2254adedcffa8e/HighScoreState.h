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
		class HighScoreState : public MenuState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);

			static HighScoreState& getInstance() {
				static HighScoreState _instance;
				return _instance;
			}

		protected:
			HighScoreState() { }

		private:
			GameBase *_game;
			Menu *_menu;
			std::vector<std::vector<std::string>> *_highscores;

			void loadText(std::string key, std::string text, std::string fontName, int fontSize);
		};
	}
}