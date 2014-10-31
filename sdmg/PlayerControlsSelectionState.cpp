#include "PlayerControlsSelectionState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\input\InputEngine.h"
#include "gamestates\MainMenuState.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"

namespace sdmg {
	namespace gamestates {

		void PlayerControlsSelectionState::init(GameBase &game)
		{
			_game = &game;

			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - 187.5f, 100);

			// Create menu item
			helperclasses::menuitems::MenuTextItem *right = new helperclasses::menuitems::MenuTextItem("Walk Right", 0, 68, true);
			right->loadText(_game, "keyboard", "Keyboard", "trebucbd", 33);
			_menu->addMenuItem(right);

			game.getEngine()->getDrawEngine()->load("controls", R"(assets\screens\help)");
		}

		void PlayerControlsSelectionState::menuAction(MenuItem *item)
		{
			std::string tag = item->getTag();

			if (tag == "Roll") {
				changeState(*_game, MainMenuState::getInstance());
			}
		}

		void PlayerControlsSelectionState::cleanup(GameBase &game)
		{

		}

		void PlayerControlsSelectionState::pause(GameBase &game)
		{
		}

		void PlayerControlsSelectionState::resume(GameBase &game)
		{
		}

		void PlayerControlsSelectionState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			if (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					game.stop();
				}

				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						changeState(game, MainMenuState::getInstance());
						break;
					case SDLK_1:
						std::cout << "Key 1 pressed. Switching State.. " << std::endl;
						//changeState(game, LoadingState::getInstance());
						break;
					case SDLK_DOWN:
						_menu->selectNext();
						break;
					case SDLK_UP:
						_menu->selectPrevious();
						break;
					case SDLK_KP_ENTER:
					case SDLK_RETURN:
						menuAction(_menu->getSelectedMenuItem());
						break;
					}
				}
			}
		}

		void PlayerControlsSelectionState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void PlayerControlsSelectionState::draw(GameBase &game, GameTime &gameTime)
		{
			DrawEngine *drawEngine = _game->getEngine()->getDrawEngine();

			drawEngine->prepareForDraw();
			drawEngine->draw("controls");

			_menu->draw(&game);

			drawEngine->render();
		}
	}
}