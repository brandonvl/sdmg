#include "StatisticsState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\input\InputEngine.h"
#include "OptionsState.h"

namespace sdmg {
	namespace gamestates {

		void StatisticsState::init(GameBase &game)
		{
			_game = &game;

			game.getEngine()->getDrawEngine()->load("help", R"(assets\screens\help)");

			loadText("title", "Statistics", "trebucbd", 48);

			loadText("wins", "Wins", "trebucbd", 36);
			loadText("losses", "Losses", "trebucbd", 36);

			loadText("nivekname", "Nivek the Assassin", "trebucbd", 36);
			loadText("nivekwins", "0", "trebucbd", 36);
			loadText("niveklosses", "0", "trebucbd", 36);
			loadText("silencename", "Silence the Code Ninja", "trebucbd", 36);
			loadText("silencewins", "0", "trebucbd", 36);
			loadText("silencelosses", "0", "trebucbd", 36);
			loadText("fiatname", "Fiat Panda", "trebucbd", 36);
			loadText("fiatwins", "0", "trebucbd", 36);
			loadText("fiatlosses", "0", "trebucbd", 36);
			loadText("luckyname", "Always Lucky Cowboy", "trebucbd", 36);
			loadText("luckywins", "0", "trebucbd", 36);
			loadText("luckylosses", "0", "trebucbd", 36);
			loadText("mindname", "Mind the Ice Mage", "trebucbd", 36);
			loadText("mindwins", "0", "trebucbd", 36);
			loadText("mindlosses", "0", "trebucbd", 36);
			loadText("enriquename", "Enrique the Shit is Banana's", "trebucbd", 36);
			loadText("enriquewins", "0", "trebucbd", 36);
			loadText("enriquelosses", "0", "trebucbd", 36);
		}

		void StatisticsState::cleanup(GameBase &game)
		{
			game.getEngine()->getDrawEngine()->unload("help");
			game.getEngine()->getDrawEngine()->unload("title");
			game.getEngine()->getDrawEngine()->unload("wins");
			game.getEngine()->getDrawEngine()->unload("losses");
			game.getEngine()->getDrawEngine()->unload("nivekname");
			game.getEngine()->getDrawEngine()->unload("nivekwins");
			game.getEngine()->getDrawEngine()->unload("niveklosses");
			game.getEngine()->getDrawEngine()->unload("silencename");
			game.getEngine()->getDrawEngine()->unload("silencewins");
			game.getEngine()->getDrawEngine()->unload("silencelosses");
			game.getEngine()->getDrawEngine()->unload("fiatname");
			game.getEngine()->getDrawEngine()->unload("fiatwins");
			game.getEngine()->getDrawEngine()->unload("fiatlosses");
			game.getEngine()->getDrawEngine()->unload("luckyname");
			game.getEngine()->getDrawEngine()->unload("luckywins");
			game.getEngine()->getDrawEngine()->unload("luckylosses");
			game.getEngine()->getDrawEngine()->unload("mindname");
			game.getEngine()->getDrawEngine()->unload("mindwins");
			game.getEngine()->getDrawEngine()->unload("mindlosses");
			game.getEngine()->getDrawEngine()->unload("enriquename");
			game.getEngine()->getDrawEngine()->unload("enriquewins");
			game.getEngine()->getDrawEngine()->unload("enriquelosses");
		}

		void StatisticsState::pause(GameBase &game)
		{
		}

		void StatisticsState::resume(GameBase &game)
		{
		}

		void StatisticsState::handleEvents(GameBase &game, GameTime &gameTime)
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

		void StatisticsState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void StatisticsState::draw(GameBase &game, GameTime &gameTime)
		{
			DrawEngine *drawEngine = _game->getEngine()->getDrawEngine();

			drawEngine->prepareForDraw();
			drawEngine->draw("help");

			drawEngine->drawText("title", 100, 100);

			int winspos = 680;
			int lossespos = 820;

			drawEngine->drawText("wins", winspos, 200);
			drawEngine->drawText("losses", lossespos, 200);

			int vpos = 248;
			winspos += 32;
			lossespos += 48;

			drawEngine->drawText("nivekname", 100, vpos);
			drawEngine->drawText("nivekwins", winspos, vpos);
			drawEngine->drawText("niveklosses", lossespos, vpos);

			vpos += 48;

			drawEngine->drawText("silencename", 100, vpos);
			drawEngine->drawText("silencewins", winspos, vpos);
			drawEngine->drawText("silencelosses", lossespos, vpos);

			vpos += 48;

			drawEngine->drawText("fiatname", 100, vpos);
			drawEngine->drawText("fiatwins", winspos, vpos);
			drawEngine->drawText("fiatlosses", lossespos, vpos);

			vpos += 48;

			drawEngine->drawText("luckyname", 100, vpos);
			drawEngine->drawText("luckywins", winspos, vpos);
			drawEngine->drawText("luckylosses", lossespos, vpos);

			vpos += 48;

			drawEngine->drawText("mindname", 100, vpos);
			drawEngine->drawText("mindwins", winspos, vpos);
			drawEngine->drawText("mindlosses", lossespos, vpos);

			vpos += 48;

			drawEngine->drawText("enriquename", 100, vpos);
			drawEngine->drawText("enriquewins", winspos, vpos);
			drawEngine->drawText("enriquelosses", lossespos, vpos);

			drawEngine->render();
		}

		void StatisticsState::loadText(std::string key, std::string text, std::string fontName, int fontSize)
		{
			_game->getEngine()->getDrawEngine()->loadText(key, text, { 255, 255, 255 }, fontName, fontSize);
		}
	}
}
