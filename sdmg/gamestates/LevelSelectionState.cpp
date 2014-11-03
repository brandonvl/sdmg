//
//
//  @ Project : SDMG
//  @ File Name : LevelSelectionState.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "LevelSelectionState.h"
#include "engine\GameTime.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"
#include "engine\input\InputEngine.h"
#include "engine\audio\AudioEngine.h"
#include <vector>

#include "LoadingState.h"
#include "MainMenuState.h"

#include <stdio.h>


namespace sdmg {
	namespace gamestates {

		void LevelSelectionState::menuAction(MenuItem *item)
		{
			std::string tag = item->getTag();

			if (tag == "Level 1") {
				changeState(*_game, LoadingState::getInstance());
			}
			else if (tag == "Level 2") {
				changeState(*_game, LoadingState::getInstance());
			}
		}

		void LevelSelectionState::init(GameBase &game)
		{
			std::vector<std::string> *loadLevels = new std::vector<std::string>;
			loadLevels->push_back("Level 1");
			loadLevels->push_back("Level 2");

			_game = &game;
			//std::function<void(MenuItem *item)> callBack = &MainMenuState::menuAction;
			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - 187.5f, game.getEngine()->getDrawEngine()->getWindowHeight() / 2);
			// Create menu item

			
			for (int i = 0; i < loadLevels->size(); i++)
			{
				std::string h = (*loadLevels)[i];
				helperclasses::menuitems::MenuTextItem *level = new helperclasses::menuitems::MenuTextItem((*loadLevels)[i], 0, 68, i == 0);
				level->loadText(_game, (*loadLevels)[i], (*loadLevels)[i], "trebucbd", 33);
				_menu->addMenuItem(level);
			}
			
			/*
			helperclasses::menuitems::MenuTextItem *level1 = new helperclasses::menuitems::MenuTextItem("Level 1", 0, 68, true);
			level1->loadText(_game, "1evel1", "Level 1", "trebucbd", 33);
			_menu->addMenuItem(level1);

			helperclasses::menuitems::MenuTextItem *level2 = new helperclasses::menuitems::MenuTextItem("Level 2", 0, 68, true);
			level2->loadText(_game, "level2", "Level 2", "trebucbd", 33);
			_menu->addMenuItem(level2);
			*/
			

			game.getEngine()->getDrawEngine()->load("background", "assets/screens/mainmenu");
		}

		void LevelSelectionState::cleanup(GameBase &game)
		{
			delete _menu;
			game.getEngine()->getDrawEngine()->unloadText("levl1");
			game.getEngine()->getDrawEngine()->unloadText("level2");
			game.getEngine()->getDrawEngine()->unloadAll();
			game.getEngine()->getInputEngine()->clearBindings();
			game.getEngine()->getAudioEngine()->unload("main_menu_bgm");
		}

		void LevelSelectionState::pause(GameBase &game)
		{
		}

		void LevelSelectionState::resume(GameBase &game)
		{
		}

		void LevelSelectionState::handleEvents(GameBase &game, GameTime &gameTime)
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

		void LevelSelectionState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void LevelSelectionState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("background");
			_menu->draw(&game);
			game.getEngine()->getDrawEngine()->render();
		}		
	}
}
