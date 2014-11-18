#include "CreditsState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\input\InputEngine.h"
#include "OptionsState.h"

namespace sdmg {
	namespace gamestates {

		void CreditsState::init(GameBase &game)
		{
			_game = &game;
			game.getEngine()->getInputEngine()->clearBindings();

			game.getEngine()->getDrawEngine()->load("credits_background", "assets/screens/mainbackground");
			
			loadText("title", "S.D.M.G.", "trebucbd", 36);
			loadText("developers", "Developers", "trebucbd", 36);
			loadText("kevin",	"Kevin Brok", "trebucbd", 36);
			loadText("mathijs", "Mathijs van Lith", "trebucbd", 36);
			loadText("brandon", "Brandon van Lent", "trebucbd", 36);
			loadText("wouter",	"Wouter van Outersterp", "trebucbd", 36);
			loadText("niek",	"Niek Willems", "trebucbd", 36);
			loadText("este",	"Esté Tigele", "trebucbd", 36);

			loadText("starring", "Starring", "trebucbd", 36);
			loadText("bob", "Bullet Bob", "trebucbd", 36);
			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void CreditsState::cleanup(GameBase &game)
		{
			game.getEngine()->getDrawEngine()->unload("credits_background");
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
		}

		void CreditsState::pause(GameBase &game)
		{
		}

		void CreditsState::resume(GameBase &game)
		{
		}

		void CreditsState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				switch (event.type) {
				case SDL_KEYDOWN:
				case SDL_MOUSEBUTTONDOWN:
					//changeState(game, OptionsState::getInstance());
					_game->getStateManager()->popState();
					break;
				case SDL_QUIT:
					game.stop();
					break;
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
			
			drawEngine->drawText("title", 100, 100);
			drawEngine->drawText("developers", 100, 200);
			drawEngine->drawText("kevin", 400, 200);
			drawEngine->drawText("mathijs", 400, 250);
			drawEngine->drawText("brandon", 400, 300);
			drawEngine->drawText("wouter", 400, 350);
			drawEngine->drawText("niek", 400, 400);
			drawEngine->drawText("este", 400, 450);
			
			drawEngine->drawText("starring", 100, 500);
			drawEngine->drawText("bob", 400, 500);

			drawEngine->render();
		}

		void CreditsState::loadText(std::string key, std::string text, std::string fontName, int fontSize) 
		{
			_game->getEngine()->getDrawEngine()->loadText(key, text, { 255, 255, 255 }, fontName, fontSize);
		}
	}
}
