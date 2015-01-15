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
#include "ProgressState.h"
#include "HelpState.h"
#include "CreditsState.h"
#include "ControlsState.h"
#include "StatisticsState.h"
#include "HighScoreState.h"
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

		void OptionsState::init(GameBase &game)
		{
			_game = &game;

			_game->getEngine()->getDrawEngine()->loadText("optionstitle", "Options", { 255, 255, 255 }, "trebucbd", 48);

			_menu = new Menu(50, 250, game);

			_menu->addMenuTextItem("Controls", (std::function<void()>)[&] { _game->getStateManager()->pushState(ControlsState::getInstance()); });
			_menu->addMenuTextItem("Save / Load", (std::function<void()>)[&] { _game->getStateManager()->pushState(ProgressState::getInstance()); });
			_menu->addMenuTextItem("Achievements", (std::function<void()>)[&] { 
				StatisticsState::getInstance().setBackName("options");
				_game->getStateManager()->pushState(StatisticsState::getInstance());
			});
			_menu->addMenuTextItem("Help", (std::function<void()>)[&] { _game->getStateManager()->pushState(HelpState::getInstance()); });
			_menu->addMenuTextItem("Back to main menu", (std::function<void()>)[&] { _game->getStateManager()->popState(); });
			game.getEngine()->getInputEngine()->setMouseEnabled();

		}

		void OptionsState::cleanup(GameBase &game)
		{
			delete _menu;
			game.getEngine()->getDrawEngine()->unloadText("controls");
			game.getEngine()->getDrawEngine()->unloadText("saveload");
			game.getEngine()->getDrawEngine()->unloadText("help");
			game.getEngine()->getDrawEngine()->unloadText("back");
			game.getEngine()->getInputEngine()->clearBindings();
		}
		
		void OptionsState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			if (SDL_PollEvent(&event))
			{
				game.getEngine()->getInputEngine()->handleEvent(event);

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
						_menu->doAction();
						break;
					}
				}
				else if (event.type == SDL_CONTROLLERBUTTONDOWN)
				{
					switch (event.cbutton.button)
					{
					case SDL_CONTROLLER_BUTTON_B:
						_game->getStateManager()->popState();
						break;
					case SDL_CONTROLLER_BUTTON_START:
						_menu->doAction();
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_UP:
						_menu->selectPrevious();
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
						_menu->selectNext();
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
			game.getEngine()->getDrawEngine()->drawText("optionstitle", 50, 70);
			_menu->draw(&game);
			game.getEngine()->getDrawEngine()->render();
		}
	}
}