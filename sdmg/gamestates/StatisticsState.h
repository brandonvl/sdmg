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
			GameBase *_game;
			std::string *_backName;

			int _screenNumber;

			void loadText(std::string key, std::string text, std::string fontName, int fontSize);
		};
	}
}