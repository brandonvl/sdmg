//
//
//  @ Project : SDMG
//  @ File Name : GameOverState.h
//  @ Date : 10-11-2014
//  @ Author : 42IN13SAd
//
//

#include "ProgressState.h"
#include "ProgressSaveState.h"
#include "ProgressLoadState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\input\InputEngine.h"
#include "OptionsState.h"
#include "lib\JSONParser.h"
#include "helperclasses\ProgressManager.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"

namespace sdmg {
	namespace gamestates {

		void ProgressState::init(GameBase &game)
		{
			_game = &game;
			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - 187.5f, game.getEngine()->getDrawEngine()->getWindowHeight() / 2.1, game);

			game.getEngine()->getDrawEngine()->load("statics_background", "assets/screens/mainbackground");

			// Load dynamic text
			game.getEngine()->getDrawEngine()->loadDynamicText("TextAutosave", { 255, 255, 255 }, "arial", 36);

			_isEnabled = ProgressManager::getInstance().autosaveEnabled();

			// Create menu
			_menu->addMenuTextItem("Autosave", (std::function<void()>)[&] { 
				_isEnabled = !ProgressManager::getInstance().autosaveEnabled();
				ProgressManager::getInstance().setAutosave(_isEnabled);
			});
			_menu->addMenuTextItem("Save", (std::function<void()>)[&] { 
				_game->getStateManager()->pushState(ProgressSaveState::getInstance());
			});
			_menu->addMenuTextItem("Load", (std::function<void()>)[&] { 
				_game->getStateManager()->pushState(ProgressLoadState::getInstance());
			});
			_menu->addMenuTextItem("Delete", (std::function<void()>)[&] {
				//_game->getStateManager()->pushState(ProgressDeleteState::getInstance());
			});
			_menu->addMenuTextItem("Back", (std::function<void()>)[&] { _game->getStateManager()->popState(); });
			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void ProgressState::cleanup(GameBase &game)
		{
			game.getEngine()->getInputEngine()->clearBindings();
			
			delete _menu;
		}

		void ProgressState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				game.getEngine()->getInputEngine()->handleEvent(event);

				switch (event.type) {
				case SDL_QUIT:
					game.stop();
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						_game->getStateManager()->popState();
						break;
					case SDLK_DOWN:
						_menu->selectNext();
						break;
					case SDLK_UP:
						_menu->selectPrevious();
						break;
					case SDLK_KP_ENTER:
					case SDLK_RETURN:
						_menu->doAction();
						break;
					}
				}
			}
		}

		void ProgressState::update(GameBase &game, GameTime &gameTime) 
		{
			// Update
		}

		void ProgressState::draw(GameBase &game, GameTime &gameTime) 
		{
			DrawEngine *drawEngine = _game->getEngine()->getDrawEngine();

			drawEngine->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("mainmenu_background");

			_menu->draw(&game);

			game.getEngine()->getDrawEngine()->drawDynamicText("TextAutosave", _isEnabled ? "[X]" : "[  ]", 470, 352);

			drawEngine->render();
		}

		void ProgressState::loadText(std::string key, std::string text, std::string fontName, int fontSize)
		{
			_game->getEngine()->getDrawEngine()->loadText(key, text, { 255, 255, 255 }, fontName, fontSize);
		}
	}
}
