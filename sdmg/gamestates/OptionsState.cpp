//
//
//  @ Project : SDMG
//  @ File Name : MainMenuState.cpp
//  @ Date : 25-10-2014
//  @ Author : 42IN13SAd
//
//

#include "OptionsState.h"
#include "MainMenuState.h"
#include "StatisticsState.h"
#include "HelpState.h"
#include "CreditsState.h"
#include "ControlsState.h"

#include "engine\GameTime.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"
#include "engine\input\InputEngine.h"
#include "engine\audio\AudioEngine.h"
#include "actions\Actions.h"

namespace sdmg {
	namespace gamestates {

		void OptionsState::menuAction(MenuItem *item)
		{
			std::string tag = item->getTag();

			if (tag == "Controls")
			{
				_game->getStateManager()->pushState(ControlsState::getInstance());
			}
			else if (tag == "Statistics") {
				//changeState(*_game, StatisticsState::getInstance());
				_game->getStateManager()->pushState(StatisticsState::getInstance());
			}
			else if (tag == "Help") {
				_game->getStateManager()->pushState(HelpState::getInstance());
			}
			else if (tag == "Back") {
				_game->getStateManager()->popState();
				//  changeState(*_game, MainMenuState::getInstance());
			}
		}

		void OptionsState::init(GameBase &game)
		{
			_game = &game;
			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - 187.5f, game.getEngine()->getDrawEngine()->getWindowHeight() / 2);

			int height = 68;

			helperclasses::menuitems::MenuTextItem *controls = new helperclasses::menuitems::MenuTextItem("Controls", 0, height, true);
			controls->loadText(_game, "controls", "Controls", "trebucbd", 33);
			_menu->addMenuItem(controls);

			helperclasses::menuitems::MenuTextItem *statistics = new helperclasses::menuitems::MenuTextItem("Statistics", 0, height, false);
			statistics->loadText(_game, "statistics", "Statistics", "trebucbd", 33);
			_menu->addMenuItem(statistics);

			helperclasses::menuitems::MenuTextItem *help = new helperclasses::menuitems::MenuTextItem("Help", 0, height, false);
			help->loadText(_game, "help", "Help", "trebucbd", 33);
			_menu->addMenuItem(help);

			helperclasses::menuitems::MenuTextItem *back = new helperclasses::menuitems::MenuTextItem("Back", 0, height, false);
			back->loadText(_game, "back", "Back", "trebucbd", 33);
			_menu->addMenuItem(back);
		}

		void OptionsState::cleanup(GameBase &game)
		{
			delete _menu;
			game.getEngine()->getDrawEngine()->unloadText("controls");
			game.getEngine()->getDrawEngine()->unloadText("statistics");
			game.getEngine()->getDrawEngine()->unloadText("help");
			game.getEngine()->getDrawEngine()->unloadText("back");
			game.getEngine()->getInputEngine()->clearBindings();
		}

		void OptionsState::pause(GameBase &game)
		{
			std::cout << "Pausing OptionsState ... " << std::endl;
		}

		void OptionsState::resume(GameBase &game)
		{
			std::cout << "Resuming OptionsState ... " << std::endl;
		}

		void OptionsState::handleEvents(GameBase &game, GameTime &gameTime)
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
						game.getStateManager()->popState();
						//  game.getStateManager()->changeState(MainMenuState::getInstance());
						break;
					case SDLK_DOWN:
					case 1:
						_menu->selectNext();
						//game.getEngine()->getAudioEngine()->play("menu_switch_effect", 0);
						break;
					case SDLK_UP:
					case 0:
						_menu->selectPrevious();
						//game.getEngine()->getAudioEngine()->play("menu_switch_effect", 0);
						break;
					case SDLK_KP_ENTER:
					case SDLK_RETURN:
					case 10:
						menuAction(_menu->getSelectedMenuItem());
						break;
					}
				}
			}
		}

		void OptionsState::update(GameBase &game, GameTime &gameTime)
		{
			//std::cout << "Updating OptionsState ... " << std::endl;
		}

		void OptionsState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("mainmenu_background");
			_menu->draw(&game);
			game.getEngine()->getDrawEngine()->render();
		}
	}
}