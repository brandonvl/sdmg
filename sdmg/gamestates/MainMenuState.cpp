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
			game.getEngine()->getAudioEngine()->play("main_menu_bgm",0);
			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void MainMenuState::cleanup(GameBase &game)
		{
			delete _menu;
			game.getEngine()->getDrawEngine()->unloadText("play");
			game.getEngine()->getDrawEngine()->unloadText("tutorial");
			game.getEngine()->getDrawEngine()->unloadText("options");
			game.getEngine()->getDrawEngine()->unloadText("credits");
			game.getEngine()->getDrawEngine()->unloadText("quit");
			game.getEngine()->getDrawEngine()->unload("mainmenu_background");
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
			//std::cout << "Updating IntroState ... " << std::endl;
		}

		void MainMenuState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("mainmenu_background");
			_menu->draw(&game);
			//std::cout << "Draw IntroState ... " << std::endl;
			game.getEngine()->getDrawEngine()->render();
		}
	}
}