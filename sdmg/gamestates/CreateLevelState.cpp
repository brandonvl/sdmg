#include "CreateLevelState.h"
#include <sdl\include\SDL.h>
#include "engine\GameBase.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "helperclasses\Menu.h"
#include "LevelSelectionState.h"
#include "PlayState.h"

namespace sdmg {
	namespace gamestates {
		void CreateLevelState::init(GameBase &game) {
			_name = new std::string();
			_game = &game;

			game.getEngine()->getDrawEngine()->loadText("createtitle", "Enter a name for your new level", { 255, 255, 255 }, "trebucbd", 48);

			//std::function<void(MenuItem *item)> callBack = &MainMenuState::menuAction;
			_menu = new Menu(100, 250, game);

			_menu->addMenuTextItem("Create", (std::function<void()>)[&] { changeState(*_game, PlayState::getInstance()); });
			_menu->addMenuTextItem("Cancel", (std::function<void()>)[&] { _game->getStateManager()->pushState(LevelSelectionState::getInstance()); });
		}
		void CreateLevelState::cleanup(GameBase &game) {
			delete _name;
		}

		void CreateLevelState::pause(GameBase &game) {

		}
		void CreateLevelState::resume(GameBase &game) {

		}

		void CreateLevelState::handleEvents(GameBase &game, GameTime &gameTime) {

			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_KEYDOWN)
				{
					*_name += SDL_GetKeyName(event.key.keysym.sym);
				}
			}

		}
		void CreateLevelState::update(GameBase &game, GameTime &gameTime) {

		}
		void CreateLevelState::draw(GameBase &game, GameTime &gameTime) {
			game.getEngine()->getDrawEngine()->drawRectangle(Rectangle(10, 150, 300, 30), 255, 255, 255);
		}
	}
}