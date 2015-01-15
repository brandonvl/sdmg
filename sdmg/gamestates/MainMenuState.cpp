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
#include "ControlsState.h"
#include "CreditsState.h"
#include "OptionsState.h"
#include "GameModeState.h"
#include "TutorialState.h"
#include "ProgressLoadState.h"
#include "HighscoreInputState.h"
#include "HighScoreState.h"
#include "LoadingPlayBackState.h"
#include "engine\GameTime.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"
#include "helperclasses\ProgressManager.h"
#include "engine\input\InputEngine.h"
#include "engine\audio\AudioEngine.h"
#include "actions\Actions.h"
#include "engine\GameTime.h"
#include "engine\util\FileManager.h"
#include "gamestates\LevelSelectionState.h"
#include <random>

namespace sdmg {
	namespace gamestates {

		void MainMenuState::init(GameBase &game)
		{
			_game = &game;

			
			_game->getEngine()->getDrawEngine()->loadText("maintitle", "Main menu", { 255, 255, 255 }, "trebucbd", 48);

			//std::function<void(MenuItem *item)> callBack = &MainMenuState::menuAction;
			_menu = new Menu(50, 250, game);

			_menu->addMenuTextItem("Play", (std::function<void()>)[&] { changeState(*_game, GameModeState::getInstance()); });
			_menu->addMenuTextItem("Level editor", (std::function<void()>)[&] { 
				_game->setGameMode(GameBase::GameMode::Edit);
				_game->getStateManager()->changeState(LevelSelectionState::getInstance()); 
			});
			_menu->addMenuTextItem("Options", (std::function<void()>)[&] { _game->getStateManager()->pushState(OptionsState::getInstance()); });
			_menu->addMenuTextItem("Credits", (std::function<void()>)[&] { _game->getStateManager()->pushState(CreditsState::getInstance()); });
			_menu->addMenuTextItem("Playback", (std::function<void()>)[&] {
				_game->setGameMode(GameBase::GameMode::Playback);
				LoadingPlayBackState::getInstance().setPlaybackFileName("recording");
				_game->getStateManager()->changeState(LoadingPlayBackState::getInstance());
			});
			_menu->addMenuTextItem("Quit", (std::function<void()>)[&] {
				_game->stop();
			});

			game.getEngine()->getAudioEngine()->load("main_menu_bgm", "assets/sounds/mainmenu/bgm.mp3", AUDIOTYPE::MUSIC);
			//game.getEngine()->getAudioEngine()->load("menu_switch_effect", R"(assets/sounds/effects/menu_sound3.ogg)", AUDIOTYPE::SOUND_EFFECT);
			game.getEngine()->getAudioEngine()->play("main_menu_bgm", 0);
			game.getEngine()->getInputEngine()->setMouseEnabled();

			game.getEngine()->getDrawEngine()->load("mainmenu_background", "assets/screens/mainmenu");
			//game.getEngine()->getDrawEngine()->load("menu_play", "assets/screens/main/play");
			//game.getEngine()->getDrawEngine()->load("menu_options", "assets/screens/main/options");
			//game.getEngine()->getDrawEngine()->load("menu_credits", "assets/screens/mainm/credits");
			//game.getEngine()->getDrawEngine()->load("menu_quit", "assets/screens/main/quit");


			_advertisementIndex = -1;
			_advertisementRefreshRate = 15 * 10000;
			_lastTimeSinceAdvertisementChange = 0;

			if (_advertismentList != nullptr) {
				delete _advertismentList;
				_advertismentList = nullptr;
			}

			_advertismentList = new std::vector<std::string>(util::FileManager::getInstance().getFiles("assets/advertisements/"));

			if (_advertismentList->size() > 0)
			{
				loadAdvertisement();
				_shouldRefreshAdvertisement = _advertismentList->size() > 1;
			}

			// Load game
			if (ProgressManager::getInstance().currentSavegame < 0)
				_game->getStateManager()->pushState(ProgressLoadState::getInstance());
		}

		void MainMenuState::cleanup(GameBase &game)
		{
			delete _menu;
			_menu = nullptr;
			delete _advertismentList;
			_advertismentList = nullptr;

			game.getEngine()->getDrawEngine()->unloadAll();
			game.getEngine()->getInputEngine()->clearBindings();
		}

		void MainMenuState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				game.getEngine()->getInputEngine()->handleEvent(event);

				switch (event.type) {
				case SDLK_ESCAPE:
					game.stop();
					break;
				case SDL_CONTROLLERBUTTONDOWN:
					switch (event.cbutton.button)
					{
					case SDL_CONTROLLER_BUTTON_A:
						_menu->doAction();
						break;
					case SDL_CONTROLLER_BUTTON_B:
						game.stop();
					case SDL_CONTROLLER_BUTTON_DPAD_UP:
						_menu->selectPrevious();
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
						_menu->selectNext();
						break;
					}
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_DOWN:
					case 1:
						if (_menu)
							_menu->selectNext();
						//game.getEngine()->getAudioEngine()->play("menu_switch_effect", 0);
						break;
					case SDLK_UP:
					case 0:
						if (_menu)
							_menu->selectPrevious();
						//game.getEngine()->getAudioEngine()->play("menu_switch_effect", 0);
						break;
					case SDLK_KP_ENTER:
					case SDLK_RETURN:
					case 10:
						if (_menu)
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

			game.getEngine()->getDrawEngine()->drawText("maintitle", 50, 70);

			//game.getEngine()->getDrawEngine()->draw("menu_play", 585, 243);
			game.getEngine()->getDrawEngine()->draw("advertisement", _advertisementX, _advertisementY);

			if (_advertisementIndex >= 0)
				//game.getEngine()->getDrawEngine()->draw("advertisement", _advertisementX, _advertisementY);

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
			_game->getEngine()->getDrawEngine()->load("advertisement", "assets\\advertisements\\" + _advertismentList->at(advertisementIndex));

			const std::array<int, 2> size = _game->getEngine()->getDrawEngine()->getImageSize("advertisement");
			_advertisementX = _advertisementStartX + (size[0] / 2);
			_advertisementY = _advertisementStartY + (size[1] / 2);
		}
	}
}