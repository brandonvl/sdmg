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
			_menu = new Menu(50, 250, game);
			
			loadText("selectsavetitle", "Select Save Slot", "trebucbd", 48);
			
			_menu->addMenuTextItem(ProgressManager::getInstance().getSaveGameTimestamp(0) != "" ? ProgressManager::getInstance().getSaveGameTimestamp(0) : "Empty slot", (std::function<void()>)[&] {
				ProgressManager::getInstance().currentSavegame = 0;
				ProgressManager::getInstance().setTimestamp(ProgressManager::getInstance().getTimestampNow());
				ProgressManager::getInstance().save();
				changeState(game, getInstance());
			});
			_menu->addMenuTextItem(ProgressManager::getInstance().getSaveGameTimestamp(1) != "" ? ProgressManager::getInstance().getSaveGameTimestamp(1) : "Empty slot", (std::function<void()>)[&] {
				ProgressManager::getInstance().currentSavegame = 1;
				ProgressManager::getInstance().setTimestamp(ProgressManager::getInstance().getTimestampNow());
				ProgressManager::getInstance().save();
				changeState(game, getInstance());
			});
			_menu->addMenuTextItem(ProgressManager::getInstance().getSaveGameTimestamp(2) != "" ? ProgressManager::getInstance().getSaveGameTimestamp(2) : "Empty slot", (std::function<void()>)[&] {
				ProgressManager::getInstance().currentSavegame = 2;
				ProgressManager::getInstance().setTimestamp(ProgressManager::getInstance().getTimestampNow());
				ProgressManager::getInstance().save();
				changeState(game, getInstance());
			});
			_menu->addMenuTextItem("Back to progress", (std::function<void()>)[&] { _game->getStateManager()->popState(); });
			game.getEngine()->getInputEngine()->setMouseEnabled();

		}

		void ProgressSaveState::cleanup(GameBase &game) {
			delete _menu;
			game.getEngine()->getDrawEngine()->unloadText("selectsavetitle");
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

			game.getEngine()->getDrawEngine()->drawText("selectsavetitle", 50, 70);

			_menu->draw(&game);
			game.getEngine()->getDrawEngine()->render();
		}

		void ProgressSaveState::loadText(std::string key, std::string text, std::string fontName, int fontSize)
		{
			_game->getEngine()->getDrawEngine()->loadText(key, text, { 255, 255, 255 }, fontName, fontSize);
		}
	}
}
