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
#include "LoadingState.h"
#include "LoadingSinglePlayerState.h"
#include "MainMenuState.h"
#include "lib\JSONParser.h"
#include "engine\util\FileManager.h"

#include <vector>
#include <fstream>

namespace sdmg {
	namespace gamestates {

		void LevelSelectionState::init(GameBase &game)
		{
			_game = &game;
			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - 187.5f, game.getEngine()->getDrawEngine()->getWindowHeight() / 2, game);
			
			std::vector<std::string> levelList = std::vector<std::string>(util::FileManager::getInstance().getFiles("assets/levels/"));

			for (size_t i = 0; i < levelList.size(); i++)
			{
				const std::string levelFolder = levelList[i];

				std::ifstream ifile("assets/levels/" + levelFolder + "/data");

				if (!ifile.fail())
				{
					JSON::JSONDocument *doc = JSON::JSONDocument::fromFile("assets/levels/" + levelFolder + "/data");
					JSON::JSONObject &obj = doc->getRootObject();

					_menu->addMenuTextItem(obj.getString("name"), (std::function<void()>)[&, levelFolder] {

						LoadingState::getInstance().setIsTutorial(false);
						LoadingState::getInstance().setLevel(new std::string(levelFolder));
						changeState(*_game, LoadingState::getInstance());
						/*
						LoadingSinglePlayerState::getInstance().setPlayer("nivek");
						changeState(*_game, LoadingSinglePlayerState::getInstance());
						*/
					});

					delete doc;
				}
			}
			
			game.getEngine()->getDrawEngine()->load("levelselect_background", "assets/screens/mainmenu");
			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void LevelSelectionState::cleanup(GameBase &game)
		{
			delete _menu;
			game.getEngine()->getDrawEngine()->unloadAll();
			game.getEngine()->getInputEngine()->clearBindings();
		}
		
		void LevelSelectionState::handleEvents(GameBase &game, GameTime &gameTime)
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
						changeState(*_game, MainMenuState::getInstance());
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
			}
		}

		void LevelSelectionState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void LevelSelectionState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("levelselect_background");
			_menu->draw(&game);
			game.getEngine()->getDrawEngine()->render();
		}
	}
}
