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
#include "CharacterSelectionState.h"

#include <vector>

namespace sdmg {
	namespace gamestates {

		void GameModeState::init(GameBase &game)
		{
			_game = &game;
			_menu = new Menu(50, 250, game);
			_game->getEngine()->getDrawEngine()->loadText("gamemodetitle", "Select a game mode", { 255, 255, 255 }, "trebucbd", 48);

			_menu->addMenuTextItem("Versus", (std::function<void()>)[&] {
				_game->setGameMode(GameBase::GameMode::Versus);
				changeState(*_game, CharacterSelectionState::getInstance());
			});
			_menu->addMenuTextItem("Singleplayer", (std::function<void()>)[&] {
				_game->setGameMode(GameBase::GameMode::SinglePlayer);
				changeState(*_game, CharacterSelectionState::getInstance());
			});
			_menu->addMenuTextItem("Survival", (std::function<void()>)[&] {
				_game->setGameMode(GameBase::GameMode::Survival);
				changeState(*_game, CharacterSelectionState::getInstance());
			});
			_menu->addMenuTextItem("Tutorial", (std::function<void()>)[&] {
				LoadingState::getInstance().setIsTutorial(true);
				LoadingState::getInstance().setLevel(new std::string("tutorial"));
				changeState(*_game, LoadingState::getInstance());
			});
			_menu->addMenuTextItem("Back to Main menu", (std::function<void()>)[&] {
				changeState(*_game, MainMenuState::getInstance());
			});

			game.getEngine()->getDrawEngine()->load("gamemodeselect_background", "assets/screens/mainmenu");
			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void GameModeState::cleanup(GameBase &game)
		{
			delete _menu;
			_menu = nullptr;
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
				else if (event.type == SDL_CONTROLLERBUTTONDOWN)
				{
					switch (event.cbutton.button)
					{
					case SDL_CONTROLLER_BUTTON_B:
						changeState(*_game, MainMenuState::getInstance());
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

		void GameModeState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void GameModeState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("gamemodeselect_background");
			game.getEngine()->getDrawEngine()->drawText("gamemodetitle", 50, 70);
			_menu->draw(&game);
			game.getEngine()->getDrawEngine()->render();
		}
	}
}