//
//
//  @ Project : SDMG
//  @ File Name : GameOverState.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#include "GameOverState.h"
#include "engine\GameTime.h"
#include "engine\Engine.h"
#include "engine\input\InputEngine.h"
#include "engine\drawing\DrawEngine.h"
#include "model\Character.h"
#include "LoadingState.h"
#include "MainMenuState.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"

namespace sdmg {
	namespace gamestates {
		void GameOverState::init(GameBase &game)
		{
			_game = &game;
			std::cout << "Initing IntroState ... " << std::endl;

			//  _menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - 187.5f, game.getEngine()->getDrawEngine()->getWindowHeight() / 2);
			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() - (187.5f * 3), 50.0f);
			// Create menu item
			helperclasses::menuitems::MenuTextItem *play = new helperclasses::menuitems::MenuTextItem("Replay", 0, 68, true);
			play->loadText(_game, "replay", "Replay", "trebucbd", 33);
			_menu->addMenuItem(play);

			helperclasses::menuitems::MenuTextItem *quit = new helperclasses::menuitems::MenuTextItem("Main Menu", 0, 68, false);
			quit->loadText(_game, "main menu", "Main Menu", "trebucbd", 33);
			_menu->addMenuItem(quit);

			std::string first = (*_characters)[_wonCharacter]->getName();
			std::string second = (*_characters)[0]->getName() == first ? (*_characters)[1]->getName() : (*_characters)[0]->getName();
			std::string third = "Mind The Ice Mage";
			std::string fourth = "Enrique The Suit Is Banana's Maso";

			game.getEngine()->getDrawEngine()->loadText("first", "1. " + first, { 255, 255, 255 }, { 0, 0, 0 }, "arial", 72);
			game.getEngine()->getDrawEngine()->loadText("second", "2. " + second, { 63.75, 63.75, 63.75 }, { 0, 0, 0 }, "arial", 54);
			game.getEngine()->getDrawEngine()->loadText("third", "3. " + third, { 128.50, 128.50, 128.50 }, { 0, 0, 0 }, "arial", 36);
			game.getEngine()->getDrawEngine()->loadText("fourth", "4. " + fourth, { 191.25, 191.25, 191.25 }, { 0, 0, 0 }, "arial", 18);

			game.getEngine()->getDrawEngine()->load("winner", "assets/characters/" + first + "/win.sprite");
			game.getEngine()->getDrawEngine()->load("background", "assets/screens/gameover");

		}

		void GameOverState::menuAction(MenuItem *item)
		{
			std::string tag = item->getTag();

			if (tag == "Replay") {
				// changeState(*_game, PlayState::getInstance());
				changeState(*_game, LoadingState::getInstance());
			}
			else if (tag == "Main Menu") {
				changeState(*_game, MainMenuState::getInstance());
			}
		}

		void GameOverState::setCharacters(std::vector<model::Character*> *characters)
		{
			_characters = characters;

			if ((*_characters)[0]->getLives() > 0)
				_wonCharacter = 0;
			else
				_wonCharacter = 1;
		}

		void GameOverState::cleanup(GameBase &game)
		{
			game.getEngine()->getDrawEngine()->unloadAll();
			game.getEngine()->getInputEngine()->clearBindings();
		}

		void GameOverState::pause(GameBase &game)
		{
			std::cout << "Pausing GameOverState ... " << std::endl;
		}

		void GameOverState::resume(GameBase &game)
		{
			std::cout << "Resuming GameOverState ... " << std::endl;
		}

		void GameOverState::handleEvents(GameBase &game, GameTime &gameTime)
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

		void GameOverState::update(GameBase &game, GameTime &gameTime)
		{
			//std::cout << "Updating GameOverState ... " << std::endl;
		}

		void GameOverState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("background");
			game.getEngine()->getDrawEngine()->draw("winner", 190, 190);

			game.getEngine()->getDrawEngine()->drawText("first", 816, 280);
			game.getEngine()->getDrawEngine()->drawText("second", 704, 380);
			game.getEngine()->getDrawEngine()->drawText("third", 592, 480);
			game.getEngine()->getDrawEngine()->drawText("fourth", 480, 580);

			_menu->draw(&game);
			game.getEngine()->getDrawEngine()->render();
		}
	}
}
