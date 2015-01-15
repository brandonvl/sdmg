//
//
//  @ Project : SDMG
//  @ File Name : PauseState.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "PauseState.h"
#include "PlayState.h"
#include "MainMenuState.h"
#include "GameOverState.h"
#include "GameOverSurvivalState.h"
#include "LoadingSinglePlayerState.h"
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
#include "engine\GameTime.h"
#include "engine\util\FileManager.h"
#include "engine\particle\ParticleEngine.h"

namespace sdmg {
	namespace gamestates {

		void PauseState::returnToMainMenu()
		{
			_game->getEngine()->getParticleEngine()->unloadAll();
			if (_game->getGameMode() == GameBase::GameMode::Survival)
				GameOverSurvivalState::getInstance().cleanup(*_game);
			else if (_game->getGameMode() == GameBase::GameMode::SinglePlayer)
			{
				LoadingSinglePlayerState::getInstance().unloadAll();
				GameOverState::getInstance().cleanup(*_game);
			}
			else
			{
				GameOverState::getInstance().cleanup(*_game);
				PlayState::getInstance().cleanup(*_game);
			}
			_game->getStateManager()->changeState(MainMenuState::getInstance());
		}

		void PauseState::returnToLevelSelection()
		{
			_game->getEngine()->getParticleEngine()->unloadAll();
			GameOverState::getInstance().cleanup(*_game);
			PlayState::getInstance().cleanup(*_game);
			_game->getStateManager()->changeState(LevelSelectionState::getInstance());
		}

		void PauseState::init(GameBase &game)
		{
			_game = &game;

			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - 187.5f, 300, game);

			std::function<void()> callBackMainMenu = std::bind(&PauseState::returnToMainMenu, this);
			std::function<void()> callBackLevelSelection = std::bind(&PauseState::returnToLevelSelection, this);

			_menu->addMenuTextItem("Resume", (std::function<void()>)[&] { _game->getStateManager()->popState(); });
			/*
			if (game.getGameMode() == GameBase::GameMode::Versus)
				_menu->addMenuTextItem("Level selection", callBackLevelSelection);
			*/
			_menu->addMenuTextItem("Main menu", callBackMainMenu);

			game.getEngine()->getDrawEngine()->loadText("pause", "Pause", { 255, 255, 255 }, "arial", 70);

			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void PauseState::cleanup(GameBase &game)
		{
			delete _menu;
			_menu = nullptr;
			game.getEngine()->getInputEngine()->getMouse().clear();
		}

		void PauseState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			if (SDL_PollEvent(&event))
			{
				game.getEngine()->getInputEngine()->getMouse().handleMouseEvent(event);

				if (event.type == SDL_QUIT)
				{
					game.stop();
				}

				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						game.getStateManager()->popState();
						break;
					case SDLK_DOWN:
					case 1:
						_menu->selectNext();
						break;
					case SDLK_UP:
					case 0:
						_menu->selectPrevious();
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
						game.getStateManager()->popState();
						break;
					case SDL_CONTROLLER_BUTTON_A:
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

		void PauseState::update(GameBase &game, GameTime &gameTime)
		{
			//game.getEngine()->getInputEngine()->update(game);
		}

		void PauseState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			PlayState::getInstance().performDraw(game);
			_game->getEngine()->getDrawEngine()->drawText("pause", (_game->getEngine()->getDrawEngine()->getWindowWidth() - _game->getEngine()->getDrawEngine()->getTextSize("pause")[0]) / 2, 200);
			_menu->draw(&game);
			game.getEngine()->getDrawEngine()->render();
		}
	}
}
