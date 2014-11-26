//
//
//  @ Project : SDMG
//  @ File Name : GameOverState.h
//  @ Date : 10-11-2014
//  @ Author : 42IN13SAd
//
//

#include "ProgressState.h"
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
			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() - (187.5f * 3), 200.0f, game);

			game.getEngine()->getDrawEngine()->load("statics_background", "assets/screens/mainbackground");

			// Load header text
			loadText("title", "Progress", "trebucbd", 48);

			_menu->addMenuTextItem("Autosave", (std::function<void()>)[&] { ProgressManager::getInstance().setAutosave(!ProgressManager::getInstance().autosaveEnabled()); });
			_menu->addMenuTextItem("Save", (std::function<void()>)[&] { ProgressManager::getInstance().save(); });
			_menu->addMenuTextItem("Load", (std::function<void()>)[&] { ProgressManager::getInstance().load(); });
			_menu->addMenuTextItem("Delete", (std::function<void()>)[&] { 
				ProgressManager::getInstance().reset();
				ProgressManager::getInstance().save(); 
			});
			_menu->addMenuTextItem("Back", (std::function<void()>)[&] { _game->getStateManager()->popState(); });
			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void ProgressState::cleanup(GameBase &game)
		{
			game.getEngine()->getDrawEngine()->unload("statics_background");
			game.getEngine()->getDrawEngine()->unload("title");
			
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
		}

		void ProgressState::draw(GameBase &game, GameTime &gameTime)
		{

			DrawEngine *drawEngine = _game->getEngine()->getDrawEngine();

			drawEngine->prepareForDraw();
			drawEngine->draw("statics_background");

			drawEngine->drawText("title", 100, 100);

			_menu->draw(&game);

			drawEngine->render();
		}

		void ProgressState::loadText(std::string key, std::string text, std::string fontName, int fontSize)
		{
			_game->getEngine()->getDrawEngine()->loadText(key, text, { 255, 255, 255 }, fontName, fontSize);
		}
	}
}
