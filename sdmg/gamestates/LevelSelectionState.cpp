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
#include "MainMenuState.h"
#include "lib\JSONParser.h"

#include <vector>


// only for windows
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

namespace sdmg {
	namespace gamestates {

		void LevelSelectionState::menuAction()
		{
			MenuItem *item = _menu->getSelectedMenuItem();
			std::string tag = item->getTag();

			if (tag == "Tutorial") {
				LoadingState::getInstance().setIsTutorial(true);
				LoadingState::getInstance().setLevel(new std::string("level1"));
				changeState(*_game, LoadingState::getInstance());
			}
			else
			{
				LoadingState::getInstance().setIsTutorial(false);
				LoadingState::getInstance().setLevel(new std::string(item->getTag()));
				changeState(*_game, LoadingState::getInstance());
			}
		}

		void LevelSelectionState::listLevels(std::function<void()> &callback) {
			std::vector<std::string> names;
			char search_path[200];
			sprintf_s(search_path, "%s*.*", "assets\\levels\\");
			WIN32_FIND_DATA fd;
			HANDLE hFind = ::FindFirstFile(search_path, &fd);
			if (hFind != INVALID_HANDLE_VALUE)
			{
				do {
					if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && fd.cFileName[0] != '.') {
						std::string s = fd.cFileName;
						JSON::JSONDocument *doc = JSON::JSONDocument::fromFile("assets/levels/" + s + "/data");
						JSON::JSONObject &obj = doc->getRootObject();

						_menu->addMenuTextItem(obj.getString("name"), callback);

						delete doc;
					}
				} while (::FindNextFile(hFind, &fd));
				::FindClose(hFind);
			}
		}

		void LevelSelectionState::init(GameBase &game)
		{
			_game = &game;
			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - 187.5f, game.getEngine()->getDrawEngine()->getWindowHeight() / 2, game);
			
			std::function<void()> callback = std::bind(&LevelSelectionState::menuAction, this);
			listLevels(callback);

			_menu->addMenuTextItem("Tutorial", callback);

			game.getEngine()->getDrawEngine()->load("levelselect_background", "assets/screens/mainmenu");
			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void LevelSelectionState::cleanup(GameBase &game)
		{
			delete _menu;
			game.getEngine()->getDrawEngine()->unloadText("Level 1");
			game.getEngine()->getDrawEngine()->unloadText("Level 2");
			game.getEngine()->getDrawEngine()->unloadText("tutorial");
			game.getEngine()->getDrawEngine()->unloadText("levelselect_background");
			game.getEngine()->getDrawEngine()->unloadAll();
			game.getEngine()->getInputEngine()->clearBindings();
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
						//  changeState(game, MainMenuState::getInstance());
						//_game->getStateManager()->cleanup();
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
						menuAction();
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
