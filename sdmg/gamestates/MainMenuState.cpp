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
#include "engine\GameTime.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"
#include "engine\input\InputEngine.h"
#include "actions\Actions.h"

namespace sdmg {
	namespace gamestates {

		void MainMenuState::menuAction(MenuItem *item)
		{
			std::string tag = item->getTag();

			if(tag == "Play") {
				// changeState(*_game, PlayState::getInstance());
				changeState(*_game, LoadingState::getInstance());
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

			helperclasses::menuitems::MenuTextItem *quit = new helperclasses::menuitems::MenuTextItem("Quit", 0, 68, false);
			quit->loadText(_game, "quit", "Quit", "trebucbd", 33);
			_menu->addMenuItem(quit);

			std::cout << "Initing IntroState ... " << std::endl;

			game.getEngine()->getDrawEngine()->load("background", "assets/screens/mainmenu");
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
