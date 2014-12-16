//
//
//  @ Project : SDMG
//  @ File Name : GameModeState.cpp
//  @ Date : 8-12-2014
//  @ Author : 42IN13SAd
//
//

#include "GameModeState.h"
#include "engine\GameTime.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"
#include "engine\input\InputEngine.h"
#include "engine\audio\AudioEngine.h"
#include "LoadingSinglePlayerState.h"
#include "LoadingSurvivalState.h"
#include "LoadingState.h"
#include "LevelSelectionState.h"
#include "MainMenuState.h"

#include <vector>

namespace sdmg {
	namespace gamestates {

		void GameModeState::init(GameBase &game)
		{
			_game = &game;
			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - 187.5f, game.getEngine()->getDrawEngine()->getWindowHeight() / 2, game);

			_menu->addMenuTextItem("Versus", (std::function<void()>)[&] {
				_game->setGameMode(GameBase::GameMode::Versus);
				changeState(*_game, LevelSelectionState::getInstance());
			});
			_menu->addMenuTextItem("Singleplayer", (std::function<void()>)[&] {
				_game->setGameMode(GameBase::GameMode::SinglePlayer);
				LoadingSinglePlayerState::getInstance().setPlayerName("nivek");
				changeState(*_game, LoadingSinglePlayerState::getInstance());
				LoadingSinglePlayerState::getInstance().loadNextFight();
			});
			_menu->addMenuTextItem("Survival", (std::function<void()>)[&] {
				_game->setGameMode(GameBase::GameMode::Survival);
				LoadingSurvivalState::getInstance().setPlayerName("nivek");
				changeState(*_game, LoadingSurvivalState::getInstance());
			});
			_menu->addMenuTextItem("Tutorial", (std::function<void()>)[&] {
				LoadingState::getInstance().setIsTutorial(true);
				LoadingState::getInstance().setLevel(new std::string("level1"));
				changeState(*_game, LoadingState::getInstance());
			});

			game.getEngine()->getDrawEngine()->load("gamemodeselect_background", "assets/screens/mainmenu");
			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void GameModeState::cleanup(GameBase &game)
		{
			delete _menu;
			game.getEngine()->getDrawEngine()->unloadAll();
			game.getEngine()->getInputEngine()->clearBindings();
		}

		void GameModeState::handleEvents(GameBase &game, GameTime &gameTime)
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

		void GameModeState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void GameModeState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("gamemodeselect_background");
			_menu->draw(&game);
			game.getEngine()->getDrawEngine()->render();
		}
	}
}