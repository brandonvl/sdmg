//
//
//  @ Project : SDMG
//  @ File Name : UnlockedState.cpp
//  @ Date : 9-12-2014
//  @ Author : 42IN13SAd
//
//


#include "UnlockedState.h"
#include "engine\GameTime.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"
#include "engine\input\InputEngine.h"
#include "engine\audio\AudioEngine.h"
#include "engine\util\FileManager.h"

#include <random>

namespace sdmg {
	namespace gamestates {

		void UnlockedState::init(GameBase &game)
		{
			_game = &game;

			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - 187.5f, 550, game);
			_menu->addMenuTextItem("Back", (std::function<void()>)[&] { _game->getStateManager()->popState(); });

			DrawEngine *de = _game->getEngine()->getDrawEngine();

			de->loadText("unlocked_text", *_playerName + " and " + *_playerName + "'s level", { 255, 255, 255 }, "Arial", 24);
			de->load("unlocked_background", "assets/screens/unlocked");
			de->load("character", "assets/characters/" + *_playerName + "/win");
			de->load("level", "assets/levels/" + *_levelName + "/preview");
			
			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void UnlockedState::cleanup(GameBase &game)
		{
			delete _menu;
			delete _playerName;
			delete _levelName;
			
			game.getEngine()->getDrawEngine()->unloadText("unlocked_text");
			game.getEngine()->getDrawEngine()->unload("unlocked_background");
			game.getEngine()->getDrawEngine()->unload("character");
			game.getEngine()->getDrawEngine()->unload("level");
			game.getEngine()->getInputEngine()->clearBindings();
		}

		void UnlockedState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			while (SDL_PollEvent(&event))
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
						game.stop();
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

		void UnlockedState::update(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getInputEngine()->update(game);
		}

		void UnlockedState::draw(GameBase &game, GameTime &gameTime)
		{
			DrawEngine *de = game.getEngine()->getDrawEngine();

			de->prepareForDraw();
			de->draw("unlocked_background");
			de->drawText("unlocked_text", de->getWindowWidth() / 2 - de->getTextSize("unlocked_text")[0] / 2 , 170);
			de->draw("character", 190, 190);
			de->draw("level", 900, 325);
			
			_menu->draw(&game);
			de->render();
		}

		void UnlockedState::setPlayerName(std::string playerName)
		{
			_playerName = new std::string(playerName);
		}

		void UnlockedState::setLevelName(std::string levelName)
		{
			_levelName = new std::string(levelName);
		}
	}
}