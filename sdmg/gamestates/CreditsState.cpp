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

			game.getEngine()->getDrawEngine()->load("credits", R"(assets\screens\credits)");
			
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
		}

		void CreditsState::cleanup(GameBase &game)
		{
			game.getEngine()->getDrawEngine()->unload("credits");
			game.getEngine()->getDrawEngine()->unload("title");
			game.getEngine()->getDrawEngine()->unload("developers");
			game.getEngine()->getDrawEngine()->unload("kevin");
			game.getEngine()->getDrawEngine()->unload("mathijs");
			game.getEngine()->getDrawEngine()->unload("brandon");
			game.getEngine()->getDrawEngine()->unload("wouter");
			game.getEngine()->getDrawEngine()->unload("credits");
			game.getEngine()->getDrawEngine()->unload("niek");
			game.getEngine()->getDrawEngine()->unload("este");
			game.getEngine()->getDrawEngine()->unload("starring");
			game.getEngine()->getDrawEngine()->unload("bob");
			
			// game.getEngine()->getDrawEngine()->unloadAll();
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
			drawEngine->draw("credits");
			
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
