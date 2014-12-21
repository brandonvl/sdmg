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
		
		protected:
			HighScoreInputState() { }

		private:
			GameBase *_game;
			std::vector<Menu*> *_keyboard;

			void loadText(std::string key, std::string text, std::string fontName, int fontSize);
			void loadDynamicText(std::string key, int r, int g, int b, std::string fontName, int fontSize);

			void initKeyboard(GameBase &game);

			std::string *_highscoreInitials;
		};
	}
}