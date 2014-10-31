#include "ControlsState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\input\InputEngine.h"
#include "gamestates\MainMenuState.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"
#include <iostream>
#include <fstream>

namespace sdmg {
	namespace gamestates {

		void ControlsState::init(GameBase &game)
		{
			_game = &game;
			
			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - 187.5f, 100);

			// Create menu item
			helperclasses::menuitems::MenuTextItem *right = new helperclasses::menuitems::MenuTextItem("Walk Right", 0, 68, true);
			right->loadText(_game, "walkright", "Walk Right", "trebucbd", 33);
			_menu->addMenuItem(right);

			helperclasses::menuitems::MenuTextItem *left = new helperclasses::menuitems::MenuTextItem("Walk Left", 0, 68, false);
			left->loadText(_game, "walkleft", "Walk Left", "trebucbd", 33);
			_menu->addMenuItem(left);

			helperclasses::menuitems::MenuTextItem *jump = new helperclasses::menuitems::MenuTextItem("Jump", 0, 68, false);
			jump->loadText(_game, "jump", "Jump", "trebucbd", 33);
			_menu->addMenuItem(jump);

			helperclasses::menuitems::MenuTextItem *roll = new helperclasses::menuitems::MenuTextItem("Roll", 0, 68, false);
			roll->loadText(_game, "roll", "Roll", "trebucbd", 33);
			_menu->addMenuItem(roll);

			game.getEngine()->getDrawEngine()->load("controls", R"(assets\screens\help)");
		}

		void ControlsState::menuAction(MenuItem *item)
		{
			std::string tag = item->getTag();

			InputDeviceBinding *binding = new InputDeviceBinding();
			if (tag == "Walk Right") {
				//action = "right";
			}
		}

		void ControlsState::cleanup(GameBase &game)
		{

		}

		void ControlsState::pause(GameBase &game)
		{
		}

		void ControlsState::resume(GameBase &game)
		{
		}

		void ControlsState::handleEvents(GameBase &game, GameTime &gameTime)
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
					if (!waiting)
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
							case SDLK_RIGHT:
								currentplayer++;
								break;
							case SDLK_LEFT:
								currentplayer--;
								break;
							case SDLK_KP_ENTER:
							case SDLK_RETURN:
								waiting = true;
								menuAction(_menu->getSelectedMenuItem());
								break;
						}
					}
					else
					{
						bindKey(event.key.keysym.sym);
					}
				}
			}
		}

		void ControlsState::bindKey(const int keyCode)
		{
			std::ofstream keybindings;
			keybindings.open("keybindings.txt");
			keybindings << currentplayer;
			keybindings << "\n";
			keybindings << _menu->getSelectedMenuItem()->getTag();
			keybindings << " : ";
			keybindings << keyCode;
			keybindings.close();
			waiting = false;
		}

		void ControlsState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void ControlsState::draw(GameBase &game, GameTime &gameTime)
		{
			DrawEngine *drawEngine = _game->getEngine()->getDrawEngine();

			drawEngine->prepareForDraw();
			drawEngine->draw("controls");

			_menu->draw(&game);

			drawEngine->render();
		}
	}
}