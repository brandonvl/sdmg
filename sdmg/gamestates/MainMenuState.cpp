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
#include "engine\GameTime.h"
#include "engine\util\FileManager.h"

#include <random>
// only for windows
// Advertisement ophalen
/*
#include <array>
#include <vector>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
*/

namespace sdmg {
	namespace gamestates {

		void MainMenuState::init(GameBase &game)
		{
			_game = &game;

			//std::function<void(MenuItem *item)> callBack = &MainMenuState::menuAction;
			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - 187.5f, game.getEngine()->getDrawEngine()->getWindowHeight() / 2, game);

			_menu->addMenuTextItem("Play", (std::function<void()>)[&] { changeState(*_game, LevelSelectionState::getInstance()); });
			_menu->addMenuTextItem("Options", (std::function<void()>)[&] { _game->getStateManager()->pushState(OptionsState::getInstance()); });
			_menu->addMenuTextItem("Credits", (std::function<void()>)[&] { _game->getStateManager()->pushState(CreditsState::getInstance()); });
			_menu->addMenuTextItem("Quit", (std::function<void()>)[&] { _game->stop(); });

			game.getEngine()->getAudioEngine()->load("main_menu_bgm", "assets/sounds/mainmenu/bgm.mp3", AUDIOTYPE::MUSIC);
			//game.getEngine()->getAudioEngine()->load("menu_switch_effect", R"(assets/sounds/effects/menu_sound3.ogg)", AUDIOTYPE::SOUND_EFFECT);
			game.getEngine()->getDrawEngine()->load("mainmenu_background", "assets/screens/mainmenu");
			game.getEngine()->getAudioEngine()->play("main_menu_bgm", 0);
			game.getEngine()->getInputEngine()->setMouseEnabled();



			_advertisementIndex = -1;
			_advertisementRefreshRate = 15 * 10000;
			_lastTimeSinceAdvertisementChange = 0;
			_advertismentList = util::FileManager::getInstance().getFiles("assets/advertisements/");

			if (_advertismentList->size() > 0)
			{
				loadAdvertisement();
				_shouldRefreshAdvertisement = _advertismentList->size() > 1;
			}
		}

		void MainMenuState::cleanup(GameBase &game)
		{
			delete _menu;

			for (auto i : *_advertismentList)
			{
				delete i;
				i = nullptr;
			}
			delete _advertismentList;
			_advertismentList = nullptr;

			game.getEngine()->getDrawEngine()->unloadAll();
			game.getEngine()->getInputEngine()->clearBindings();
		}

		void MainMenuState::handleEvents(GameBase &game, GameTime &gameTime)
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
						_menu->doAction();
						break;
					}
				}
			}
		}

		void MainMenuState::update(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getInputEngine()->update(game);

			if (_advertisementIndex >= 0.0f && _shouldRefreshAdvertisement)
			{
				int timeSinceLastUpdate = game.getGameTime()->getElapsedSinceLastUpdate() / 1000.0f;
				if (timeSinceLastUpdate >= 0.0f)
				{
					if ((_lastTimeSinceAdvertisementChange += timeSinceLastUpdate) >= _advertisementRefreshRate)
					{
						loadAdvertisement();
						_lastTimeSinceAdvertisementChange = 0.0f;
					}
				}
			}
		}

		void MainMenuState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("mainmenu_background");

			if (_advertisementIndex >= 0)
				game.getEngine()->getDrawEngine()->draw("advertisement", _advertisementX, _advertisementY);

			_menu->draw(&game);
			game.getEngine()->getDrawEngine()->render();
		}


		void MainMenuState::loadAdvertisement()
		{
			if (_advertisementIndex >= 0)
				_game->getEngine()->getDrawEngine()->unload("advertisement");

			std::random_device dev;
			std::default_random_engine dre(dev());
			std::uniform_int_distribution<int> randomAdvertisement(0, _advertismentList->size() - 1);

			int advertisementIndex = 0;
			do
				advertisementIndex = randomAdvertisement(dre);
			while (advertisementIndex == _advertisementIndex);

			_advertisementIndex = advertisementIndex;
			_game->getEngine()->getDrawEngine()->load("advertisement", "assets\\advertisements\\" + *(*_advertismentList)[advertisementIndex]);

			const std::array<float, 2> size = _game->getEngine()->getDrawEngine()->getImageSize("advertisement");
			_advertisementX = _game->getEngine()->getDrawEngine()->getWindowWidth() - size[0] - 10;
			_advertisementY = _game->getEngine()->getDrawEngine()->getWindowHeight() - size[1] - 10;
		}
	}
}