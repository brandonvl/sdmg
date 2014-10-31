//
//
//  @ Project : SDMG
//  @ File Name : MainMenuState.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "MainMenuState.h"
#include "PlayState.h"
#include "LoadingState.h"
#include "ControlsState.h"
#include "CreditsState.h"
#include "TutorialState.h"
#include "HelpState.h"
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

		void MainMenuState::menuAction(MenuItem *item)
		{
			std::string tag = item->getTag();

			if (tag == "Play") {
				// changeState(*_game, PlayState::getInstance());
				LoadingState::getInstance().setIsTutorial(false);
				changeState(*_game, LoadingState::getInstance());
			}
			else if (tag == "Tutorial") {
				LoadingState::getInstance().setIsTutorial(true);
				changeState(*_game, LoadingState::getInstance());
			}
			else if (tag == "Help") {
				changeState(*_game, HelpState::getInstance());
			}
			else if (tag == "Controls") {
				changeState(*_game, ControlsState::getInstance());
			}
			else if (tag == "Credits") {
				changeState(*_game, CreditsState::getInstance());
			}
			else if (tag == "Quit") {
				_game->stop();
			}
		}

		void MainMenuState::init(GameBase &game)
		{
			_game = &game;
			//std::function<void(MenuItem *item)> callBack = &MainMenuState::menuAction;
			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - 187.5f, game.getEngine()->getDrawEngine()->getWindowHeight() / 2);
			// Create menu item
			helperclasses::menuitems::MenuTextItem *play = new helperclasses::menuitems::MenuTextItem("Play", 0, 68, true);
			play->loadText(_game, "play", "Play", "trebucbd", 33);
			_menu->addMenuItem(play);

			helperclasses::menuitems::MenuTextItem *tutorial = new helperclasses::menuitems::MenuTextItem("Tutorial", 0, 68, false);
			tutorial->loadText(_game, "tutorial", "Tutorial", "trebucbd", 33);
			_menu->addMenuItem(tutorial);

			helperclasses::menuitems::MenuTextItem *help = new helperclasses::menuitems::MenuTextItem("Help", 0, 68, false);
			help->loadText(_game, "help", "Help", "trebucbd", 33);
			_menu->addMenuItem(help);

			helperclasses::menuitems::MenuTextItem *controls = new helperclasses::menuitems::MenuTextItem("Controls", 0, 68, false);
			controls->loadText(_game, "controls", "Controls", "trebucbd", 33);
			_menu->addMenuItem(controls);

			helperclasses::menuitems::MenuTextItem *credits = new helperclasses::menuitems::MenuTextItem("Credits", 0, 68, false);
			credits->loadText(_game, "credits", "Credits", "trebucbd", 33);
			_menu->addMenuItem(credits);

			helperclasses::menuitems::MenuTextItem *quit = new helperclasses::menuitems::MenuTextItem("Quit", 0, 68, false);
			quit->loadText(_game, "quit", "Quit", "trebucbd", 33);
			_menu->addMenuItem(quit);

			std::cout << "Initing IntroState ... " << std::endl;

			game.getEngine()->getAudioEngine()->load("main_menu_bgm", R"(assets/sounds/bgm/main_menu_bgm.mp3)", AUDIOTYPE::MUSIC);
			//game.getEngine()->getAudioEngine()->load("menu_switch_effect", R"(assets/sounds/effects/menu_sound3.ogg)", AUDIOTYPE::SOUND_EFFECT);
			game.getEngine()->getDrawEngine()->load("background", "assets/screens/mainmenu");
			// game.getEngine()->getAudioEngine()->play("main_menu_bgm",0);
		}

		void MainMenuState::cleanup(GameBase &game)
		{
			game.getEngine()->getDrawEngine()->unloadAll();
			game.getEngine()->getInputEngine()->clearBindings();
		}

		void MainMenuState::pause(GameBase &game)
		{
			std::cout << "Pausing IntroState ... " << std::endl;
		}

		void MainMenuState::resume(GameBase &game)
		{
			std::cout << "Resuming IntroState ... " << std::endl;
		}

		void MainMenuState::handleEvents(GameBase &game, GameTime &gameTime)
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
							game.stop();
							break;
						case SDLK_1:
							std::cout << "Key 1 pressed. Switching State.. " << std::endl;
							//changeState(game, LoadingState::getInstance());
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

		void MainMenuState::update(GameBase &game, GameTime &gameTime)
		{
			//std::cout << "Updating IntroState ... " << std::endl;
		}

		void MainMenuState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("background");
			_menu->draw(&game);
			//std::cout << "Draw IntroState ... " << std::endl;
			game.getEngine()->getDrawEngine()->render();
		}


	}
}