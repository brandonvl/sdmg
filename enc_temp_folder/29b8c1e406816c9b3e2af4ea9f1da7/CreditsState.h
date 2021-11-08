#pragma once
#include "MenuState.h"
#include "engine\GameState.h"

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
		class CreditsState : public MenuState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);
			
			static CreditsState& getInstance() {
				static CreditsState _instance;
				return _instance;
			}

		protected:
			CreditsState() { }
		private:
			GameBase *_game;

			void loadText(std::string key, std::string text, std::string fontName, int fontSize);
			void returnToMainMenu();
		};
	}
}

