#include "HighScoreState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\input\InputEngine.h"
#include "lib\JSONParser.h"
#include "helperclasses\ProgressManager.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"

namespace sdmg {
	namespace gamestates {

		void HighScoreState::returnToOptionsMenu()
		{
			_game->getStateManager()->popState();
		}
		void HighScoreState::init(GameBase &game)
		{
			_game = &game;
			_menu = new Menu(100, game.getEngine()->getDrawEngine()->getWindowHeight() / 2, game);
			game.getEngine()->getDrawEngine()->load("statics_background", "assets/screens/mainbackground");

			std::function<void()> CallBackOptionsMenu = std::bind(&HighScoreState::returnToOptionsMenu, this);

			// Create menu
			_menu->addMenuTextItem("Back to options", CallBackOptionsMenu);

			// Load header text
			loadText("title_highscore", "Highscores", "trebucbd", 48);

			_highscores = ProgressManager::getInstance().getHighscores();
			for (auto i = 0; i < _highscores->size(); i++) {
				loadText(("number_" + std::to_string(i)), _highscores->at(i).at(0) + ".", "trebucbd", 36);
				loadText(("name_" + std::to_string(i)), _highscores->at(i).at(1), "trebucbd", 36);
				loadText(("score_" + std::to_string(i)), _highscores->at(i).at(2), "trebucbd", 36);
			}
			game.getEngine()->getInputEngine()->setMouseEnabled();

		}

		void HighScoreState::cleanup(GameBase &game)
		{
			delete _menu;
			_menu = nullptr;
			//game.getEngine()->getDrawEngine()->unload("statics_background");
			game.getEngine()->getDrawEngine()->unload("title_highscore");

			for (auto i = 0; i < _highscores->size(); i++) {
				game.getEngine()->getDrawEngine()->unload("number_" + std::to_string(i));
				game.getEngine()->getDrawEngine()->unload("name_" + std::to_string(i));
				game.getEngine()->getDrawEngine()->unload("score_" + std::to_string(i));
			}
			delete _highscores;
			_highscores = nullptr;

			game.getEngine()->getInputEngine()->getMouse().clear();
		}

		void HighScoreState::handleEvents(GameBase &game, GameTime &gameTime)
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
					case SDLK_KP_ENTER:
					case SDLK_RETURN:
					case 10:
						_menu->doAction();
						break;
					}
				}
			}
		}

		void HighScoreState::update(GameBase &game, GameTime &gameTime)
		{

		}

		void HighScoreState::draw(GameBase &game, GameTime &gameTime)
		{
			DrawEngine *drawEngine = game.getEngine()->getDrawEngine();

			drawEngine->prepareForDraw();
			drawEngine->draw("statics_background");

			int hpos = 50;
			int vpos = 100;

			game.getEngine()->getDrawEngine()->drawText("title_highscore", vpos, hpos);

			hpos += 100;
			vpos = 700;

			for (auto i = 0; i < _highscores->size(); i++) {
				drawEngine->drawText(("number_" + std::to_string(i)), vpos, hpos);
				vpos += 100;
				drawEngine->drawText(("name_" + std::to_string(i)), vpos, hpos);
				vpos += 300;
				drawEngine->drawText(("score_" + std::to_string(i)), vpos, hpos);
				hpos += 50;
				vpos = 700;
			}

			_menu->draw(_game);

			game.getEngine()->getDrawEngine()->render();
		}

		void HighScoreState::loadText(std::string key, std::string text, std::string fontName, int fontSize)
		{
			_game->getEngine()->getDrawEngine()->loadText(key, text, { 255, 255, 255 }, fontName, fontSize);
		}
		
		/*void HighScoreState::loadDynamicText(std::string key, int r, int g, int b, std::string fontName, int fontSize)
		{
			_game->getEngine()->getDrawEngine()->loadDynamicText(key, { r, g, b }, fontName, fontSize);
		}*/
	}
}