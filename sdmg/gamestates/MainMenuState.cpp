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
#include "OptionsState.h"
#include "LevelSelectionState.h"
#include "TutorialState.h"
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
				LoadingState::getInstance().setIsTutorial(false);
				//  changeState(*_game, LoadingState::getInstance());
				changeState(*_game, LevelSelectionState::getInstance());
			}
			else if (tag == "Options") {
				changeState(*_game, OptionsState::getInstance());
			}
			else if (tag == "Credits") {
				_game->getStateManager()->pushState(CreditsState::getInstance());
				//changeState(*_game, CreditsState::getInstance());
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

			int height = 68;

			helperclasses::menuitems::MenuTextItem *play = new helperclasses::menuitems::MenuTextItem("Play", 0, height, true);
			play->loadText(_game, "play", "Play", "trebucbd", 33);
			_menu->addMenuItem(play);
			
			helperclasses::menuitems::MenuTextItem *options = new helperclasses::menuitems::MenuTextItem("Options", 0, height, false);
			options->loadText(_game, "options", "Options", "trebucbd", 33);
			_menu->addMenuItem(options);

			helperclasses::menuitems::MenuTextItem *credits = new helperclasses::menuitems::MenuTextItem("Credits", 0, height, false);
			credits->loadText(_game, "credits", "Credits", "trebucbd", 33);
			_menu->addMenuItem(credits);

			helperclasses::menuitems::MenuTextItem *quit = new helperclasses::menuitems::MenuTextItem("Quit", 0, height, false);
			quit->loadText(_game, "quit", "Quit", "trebucbd", 33);
			_menu->addMenuItem(quit);

			std::cout << "Initing IntroState ... " << std::endl;

			game.getEngine()->getAudioEngine()->load("main_menu_bgm", R"(assets/sounds/bgm/main_menu_bgm.mp3)", AUDIOTYPE::MUSIC);
			//game.getEngine()->getAudioEngine()->load("menu_switch_effect", R"(assets/sounds/effects/menu_sound3.ogg)", AUDIOTYPE::SOUND_EFFECT);
			game.getEngine()->getDrawEngine()->load("background", "assets/screens/mainmenu");
			game.getEngine()->getAudioEngine()->play("main_menu_bgm",0);
		}

		void MainMenuState::cleanup(GameBase &game)
		{
			delete _menu;
			game.getEngine()->getDrawEngine()->unloadText("play");
			game.getEngine()->getDrawEngine()->unloadText("tutorial");
			game.getEngine()->getDrawEngine()->unloadText("options");
			game.getEngine()->getDrawEngine()->unloadText("credits");
			game.getEngine()->getDrawEngine()->unloadText("quit");
			game.getEngine()->getDrawEngine()->unloadAll();
			game.getEngine()->getInputEngine()->clearBindings();
			game.getEngine()->getAudioEngine()->unload("main_menu_bgm");
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