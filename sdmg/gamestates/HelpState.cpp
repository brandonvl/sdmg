#include "HelpState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\input\InputEngine.h"
#include "MainMenuState.h"

namespace sdmg {
	namespace gamestates {

		void HelpState::init(GameBase &game)
		{
			_game = &game;

			game.getEngine()->getDrawEngine()->load("help_background", "assets/screens/mainbackground");

			loadText("title", "Help", "trebucbd", 48);
			loadText("howtowin1", "To win the game try to push your opponent of the edge", "trebucbd", 36);
			loadText("howtowin2", "or into Bullet Bob.", "trebucbd", 36);
			loadText("howtowin3", "Victory shall be yours, when your opponent runs out of", "trebucbd", 36);
			loadText("howtowin4", "lives.", "trebucbd", 36);
		}

		void HelpState::cleanup(GameBase &game)
		{
			game.getEngine()->getDrawEngine()->unload("help_background");
			game.getEngine()->getDrawEngine()->unloadText("title");
			game.getEngine()->getDrawEngine()->unloadText("howtowin1");
			game.getEngine()->getDrawEngine()->unloadText("howtowin2");
			game.getEngine()->getDrawEngine()->unloadText("howtowin3");
			game.getEngine()->getDrawEngine()->unloadText("howtowin4");
			
			//  game.getEngine()->getDrawEngine()->unloadAll();
		}

		void HelpState::pause(GameBase &game)
		{
		}

		void HelpState::resume(GameBase &game)
		{
		}

		void HelpState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				switch (event.type) {
				case SDL_KEYDOWN:
					game.getStateManager()->popState();
					break;
				case SDL_QUIT:
					game.stop();
					break;
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

			drawEngine->render();
		}

		void HelpState::loadText(std::string key, std::string text, std::string fontName, int fontSize)
		{
			_game->getEngine()->getDrawEngine()->loadText(key, text, { 255, 255, 255 }, fontName, fontSize);
		}
	}
}