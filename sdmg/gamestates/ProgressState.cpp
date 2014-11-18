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
			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() - (187.5f * 3), 50.0f);

			game.getEngine()->getDrawEngine()->load("statics_background", "assets/screens/mainbackground");

			// Load header text
			loadText("title", "Progress", "trebucbd", 48);

			// Create menu item
			helperclasses::menuitems::MenuTextItem *save = new helperclasses::menuitems::MenuTextItem("Save", 0, 68, true);
			save->loadText(_game, "save", "Save", "trebucbd", 33);
			_menu->addMenuItem(save);

			helperclasses::menuitems::MenuTextItem *load = new helperclasses::menuitems::MenuTextItem("Load", 0, 68, false);
			load->loadText(_game, "load", "Load", "trebucbd", 33);
			_menu->addMenuItem(load);

			helperclasses::menuitems::MenuTextItem *deleteGame = new helperclasses::menuitems::MenuTextItem("Delete", 0, 68, false);
			deleteGame->loadText(_game, "delete", "Delete", "trebucbd", 33);
			_menu->addMenuItem(deleteGame);

			helperclasses::menuitems::MenuTextItem *back = new helperclasses::menuitems::MenuTextItem("Back", 0, 68, false);
			back->loadText(_game, "back", "Back", "trebucbd", 33);
			_menu->addMenuItem(back);
		}

		void ProgressState::menuAction(MenuItem *item)
		{
			std::string tag = item->getTag();

			if (tag == "Save") {
				ProgressManager::getInstance().save();
			}
			else if (tag == "Load") {
				ProgressManager::getInstance().load();
			}
			else if (tag == "Delete") {
				ProgressManager::getInstance().reset();
				ProgressManager::getInstance().save();
			}
			else if (tag == "Back") {
				_game->getStateManager()->popState();
			}
		}

		void ProgressState::cleanup(GameBase &game)
		{
			game.getEngine()->getDrawEngine()->unload("statics_background");
			game.getEngine()->getDrawEngine()->unload("title");
		}

		void ProgressState::pause(GameBase &game)
		{
		}

		void ProgressState::resume(GameBase &game)
		{
		}

		void ProgressState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
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
						menuAction(_menu->getSelectedMenuItem());
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
