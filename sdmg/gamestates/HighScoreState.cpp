#include "HighScoreState.h"
#include "HighScoreInputState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\input\InputEngine.h"
#include "lib\JSONParser.h"
#include "helperclasses\ProgressManager.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"

namespace sdmg {
	namespace gamestates {

		void HighScoreState::init(GameBase &game)
		{
			_game = &game;
			_menu = new Menu(50, 250, game);
			game.getEngine()->getDrawEngine()->load("highscore_background", "assets/screens/mainmenu");

			// Create menu
			_menu->addMenuTextItem("Input highscore", (std::function<void()>)[&] { _game->getStateManager()->pushState(HighScoreInputState::getInstance()); });
			_menu->addMenuTextItem("Back to options", (std::function<void()>)[&] { _game->getStateManager()->popState(); });

			// Load header text
			loadText("highscore_title", "Highscores", "trebucbd", 48);

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
			
			game.getEngine()->getDrawEngine()->unload("highscore_background");
			game.getEngine()->getDrawEngine()->unload("highscore_title");

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
			drawEngine->draw("highscore_background");

			int hpos = 50;
			int vpos = 70;

			game.getEngine()->getDrawEngine()->drawText("highscore_title", hpos, vpos);

			hpos = 700;
			vpos = 250;

			for (auto i = 0; i < _highscores->size(); i++) {
				drawEngine->drawText(("number_" + std::to_string(i)), hpos, vpos);
				hpos += 100;
				drawEngine->drawText(("name_" + std::to_string(i)), hpos, vpos);
				hpos += 300;
				drawEngine->drawText(("score_" + std::to_string(i)), hpos, vpos);
				hpos = 700;
				vpos += 50;
			}

			_menu->draw(_game);

			game.getEngine()->getDrawEngine()->render();
		}

		void HighScoreState::loadText(std::string key, std::string text, std::string fontName, int fontSize)
		{
			_game->getEngine()->getDrawEngine()->loadText(key, text, { 255, 255, 255 }, fontName, fontSize);
		}
	}
}