#pragma once
#include "MenuState.h"
#include "engine\GameState.h"

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
		class StatisticsState :	public MenuState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);

			static StatisticsState& getInstance() {
				static StatisticsState _instance;
				return _instance;
			}

			void setBackName(std::string name);

		protected:
			StatisticsState() { }
		private:
			GameBase *_game = nullptr;
			std::string *_backName = nullptr;

			int _screenNumber;
			int _windowWidth;
			const int BULLET_SIZE = 48;
			const int BULLET_MARGIN = 10;
			const int BULLET_YPOS = 610;
			const int FIRST_SCREEN = 1;
			const int LAST_SCREEN = 2;

			void loadText(std::string key, std::string text, std::string fontName, int fontSize);
		};
	}
}