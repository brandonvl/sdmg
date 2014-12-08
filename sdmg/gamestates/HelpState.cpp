#include "HelpState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\input\InputEngine.h"
#include "MainMenuState.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"

namespace sdmg {
	namespace gamestates {

		void HelpState::returnToOptionsMenu()
		{
			_game->getStateManager()->popState();
		}

		void HelpState::init(GameBase &game)
		{
			_game = &game;

			_menu = new Menu(100, 600, game);

			std::function<void()> CallBackOptionsMenu = std::bind(&HelpState::returnToOptionsMenu, this);

			//  _menu->addMenuTextItem("Back", (std::function<void()>)[&] { _game->getStateManager()->popState(); });
			_menu->addMenuTextItem("Back", CallBackOptionsMenu);

			game.getEngine()->getDrawEngine()->load("help_background", "assets/screens/mainbackground");

			loadText("title", "Help", "trebucbd", 48);
			loadText("howtowin1", "To win the game try to push your opponent of the edge", "trebucbd", 36);
			loadText("howtowin2", "or into Bullet Bob.", "trebucbd", 36);
			loadText("howtowin3", "Victory shall be yours, when your opponent runs out of", "trebucbd", 36);
			loadText("howtowin4", "lives.", "trebucbd", 36);

			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void HelpState::cleanup(GameBase &game)
		{
			delete _menu;
			_menu = nullptr;

			game.getEngine()->getDrawEngine()->unload("help_background");
			game.getEngine()->getDrawEngine()->unloadText("title");
			game.getEngine()->getDrawEngine()->unloadText("howtowin1");
			game.getEngine()->getDrawEngine()->unloadText("howtowin2");
			game.getEngine()->getDrawEngine()->unloadText("howtowin3");
			game.getEngine()->getDrawEngine()->unloadText("howtowin4");
			
			//  game.getEngine()->getDrawEngine()->unloadAll();
			game.getEngine()->getInputEngine()->getMouse().clear();
		}

		void HelpState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				game.getEngine()->getInputEngine()->handleEvent(event);

				if (event.type == SDL_QUIT)
					game.stop();
				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
					case SDLK_KP_ENTER:
					case SDLK_RETURN:
					case 10:
						_menu->doAction();
						break;
					}
				}
			}
		}

		void HelpState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void HelpState::draw(GameBase &game, GameTime &gameTime)
		{
			DrawEngine *drawEngine = _game->getEngine()->getDrawEngine();

			drawEngine->prepareForDraw();
			drawEngine->draw("help_background");

			drawEngine->drawText("title", 100, 100);
			drawEngine->drawText("howtowin1", 100, 200);
			drawEngine->drawText("howtowin2", 100, 248);
			drawEngine->drawText("howtowin3", 100, 296);
			drawEngine->drawText("howtowin4", 100, 344);

			_menu->draw(_game);

			drawEngine->render();
		}

		void HelpState::loadText(std::string key, std::string text, std::string fontName, int fontSize)
		{
			_game->getEngine()->getDrawEngine()->loadText(key, text, { 255, 255, 255 }, fontName, fontSize);
		}
	}
}