#include "CreditsState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\input\InputEngine.h"
#include "OptionsState.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"

namespace sdmg {
	namespace gamestates {

		void CreditsState::returnToMainMenu()
		{
			_game->getStateManager()->popState();
		}

		void CreditsState::init(GameBase &game)
		{
			_game = &game;
			_game->getEngine()->getDrawEngine()->loadText("credittitle", "Credits", { 255, 255, 255 }, "trebucbd", 48);
			game.getEngine()->getInputEngine()->clearBindings();

			//std::function<void(MenuItem *item)> callBack = &MainMenuState::menuAction;
			//  _menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - 187.5f, game.getEngine()->getDrawEngine()->getWindowHeight() / 2, game);
			_menu = new Menu(50, 600, game);

			std::function<void()> callBackMainMenu = std::bind(&CreditsState::returnToMainMenu, this);
			
			//  _menu->addMenuTextItem("Back", (std::function<void()>)[&] { _game->getStateManager()->popState(); });
			_menu->addMenuTextItem("Back to main menu", callBackMainMenu);

			game.getEngine()->getDrawEngine()->load("credits_background", "assets/screens/mainmenu");
			
			loadText("developers", "Developers", "trebucbd", 34);
			loadText("kevin", "Kevin Brok", "trebuc", 34);
			loadText("mathijs", "Mathijs van Lith", "trebuc", 34);
			loadText("brandon", "Brandon van Lent", "trebuc", 34);
			loadText("wouter", "Wouter van Outersterp", "trebuc", 34);
			loadText("niek", "Niek Willems", "trebuc", 34);
			loadText("este", "Esté Tigele", "trebuc", 34);

			loadText("starring", "Starring", "trebucbd", 34);
			loadText("bob", "Bullet Bob", "trebuc", 34);

			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void CreditsState::cleanup(GameBase &game)
		{
			delete _menu;
			_menu = nullptr;

			game.getEngine()->getDrawEngine()->unload("credits_background");
			game.getEngine()->getDrawEngine()->unloadText("credittitle");
			game.getEngine()->getDrawEngine()->unloadText("title");
			game.getEngine()->getDrawEngine()->unloadText("developers");
			game.getEngine()->getDrawEngine()->unloadText("kevin");
			game.getEngine()->getDrawEngine()->unloadText("mathijs");
			game.getEngine()->getDrawEngine()->unloadText("brandon");
			game.getEngine()->getDrawEngine()->unloadText("wouter");
			game.getEngine()->getDrawEngine()->unloadText("niek");
			game.getEngine()->getDrawEngine()->unloadText("este");
			game.getEngine()->getDrawEngine()->unloadText("starring");
			game.getEngine()->getDrawEngine()->unloadText("bob");

			game.getEngine()->getInputEngine()->getMouse().clear();
		}

		void CreditsState::handleEvents(GameBase &game, GameTime &gameTime)
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

		void CreditsState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void CreditsState::draw(GameBase &game, GameTime &gameTime)
		{
			DrawEngine *drawEngine = _game->getEngine()->getDrawEngine();
			
			drawEngine->prepareForDraw();
			drawEngine->draw("credits_background");
			game.getEngine()->getDrawEngine()->drawText("credittitle", 50, 70);
			
			drawEngine->drawText("developers", 50, 250);
			drawEngine->drawText("kevin", 350, 250);
			drawEngine->drawText("mathijs", 350, 300);
			drawEngine->drawText("brandon", 350, 350);
			drawEngine->drawText("wouter", 700, 250);
			drawEngine->drawText("niek", 700, 300);
			drawEngine->drawText("este", 700, 350);
			
			drawEngine->drawText("starring", 50, 450);
			drawEngine->drawText("bob", 350, 450);

			_menu->draw(&game);
			drawEngine->render();
		}

		void CreditsState::loadText(std::string key, std::string text, std::string fontName, int fontSize) 
		{
			_game->getEngine()->getDrawEngine()->loadText(key, text, { 255, 255, 255 }, fontName, fontSize);
		}
	}
}
