//
//
//  @ Project : SDMG
//  @ File Name : ProgressSaveState.cpp
//  @ Date : 29-11-2014
//  @ Author : 42IN13SAd
//
//

#include "ProgressSaveState.h"

#include "engine\GameTime.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "helperclasses\ProgressManager.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"
#include "engine\input\InputEngine.h"
#include "engine\audio\AudioEngine.h"
#include "actions\Actions.h"

namespace sdmg {
	namespace gamestates {

		void ProgressSaveState::init(GameBase &game) {
			_game = &game;
			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - 187.5f, game.getEngine()->getDrawEngine()->getWindowHeight() / 2, game);
			
			//for (int i = 0; i < 3; i++) {
			_menu->addMenuTextItem(ProgressManager::getInstance().getSaveGameTimestamp(0) != "" ? ProgressManager::getInstance().getSaveGameTimestamp(0) : "Empty slot", (std::function<void()>)[&] {
				ProgressManager::getInstance().currentSavegame = 0;
				ProgressManager::getInstance().save();
				_game->getStateManager()->popState();

			});
			_menu->addMenuTextItem(ProgressManager::getInstance().getSaveGameTimestamp(1) != "" ? ProgressManager::getInstance().getSaveGameTimestamp(1) : "Empty slot", (std::function<void()>)[&] {
				ProgressManager::getInstance().currentSavegame = 1;
				ProgressManager::getInstance().save();
				_game->getStateManager()->popState();
			});
			_menu->addMenuTextItem(ProgressManager::getInstance().getSaveGameTimestamp(2) != "" ? ProgressManager::getInstance().getSaveGameTimestamp(2) : "Empty slot", (std::function<void()>)[&] {
				ProgressManager::getInstance().currentSavegame = 2;
				ProgressManager::getInstance().save();
				_game->getStateManager()->popState();
			});
			_menu->addMenuTextItem("Cancel", (std::function<void()>)[&] { _game->getStateManager()->popState(); });
			game.getEngine()->getInputEngine()->setMouseEnabled();

		}

		void ProgressSaveState::cleanup(GameBase &game) {
			delete _menu;
			game.getEngine()->getInputEngine()->clearBindings();
		}

		void ProgressSaveState::handleEvents(GameBase &game, GameTime &gameTime) {
			SDL_Event event;

			if (SDL_PollEvent(&event)) {
				game.getEngine()->getInputEngine()->handleEvent(event);

				switch (event.type) {
				case SDL_QUIT:
					game.stop();
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						game.getStateManager()->popState();
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
					default:
						break;
					}
				default:
					break;
				}
			}
		}

		void ProgressSaveState::update(GameBase &game, GameTime &gameTime) {
			//std::cout << "Updating ProgressSaveState ... " << std::endl;
		}

		void ProgressSaveState::draw(GameBase &game, GameTime &gameTime) {
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("mainmenu_background");
			_menu->draw(&game);
			game.getEngine()->getDrawEngine()->render();
		}
	}
}
