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

		const SDL_MessageBoxButtonData buttons[] = {
			{ 0, 0, "Yes" },
			{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Cancel" },
		};

		const SDL_MessageBoxColorScheme colorScheme = {
			{ /* .colors (.r, .g, .b) */
				/* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
				{ 255, 0, 0 },
				/* [SDL_MESSAGEBOX_COLOR_TEXT] */
				{ 0, 255, 0 },
				/* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
				{ 255, 255, 0 },
				/* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
				{ 0, 0, 255 },
				/* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
				{ 255, 0, 255 }
			}
		};

		const SDL_MessageBoxData messageboxdata = {
			SDL_MESSAGEBOX_INFORMATION, /* .flags */
			NULL, /* .window */
			"Warning!", /* .title */
			"Clear?", /* .message */
			SDL_arraysize(buttons), /* .numbuttons */
			buttons, /* .buttons */
			&colorScheme /* .colorScheme */
		};

		void HighScoreState::init(GameBase &game)
		{
			_game = &game;
			_menu = new Menu(50, 250, game);
			game.getEngine()->getDrawEngine()->load("highscore_background", "assets/screens/mainmenu");

			int buttonid;

			// Create menu
			_menu->addMenuTextItem("Back", (std::function<void()>)[&] { _game->getStateManager()->popState(); });
			_menu->addMenuTextItem("Clear highscores", (std::function<void()>)[&] { 
				if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
					SDL_Log("error displaying message box");
				}
				switch (buttonid) {
				case -1:
					SDL_Log("no selection");
					break;
				case 0: // Clear highscores
				{
					std::vector<std::vector<std::string>> *highscores = ProgressManager::getInstance().getHighscores();
					for (size_t i = 0, ilen = highscores->size(); i < ilen; ++i) {
						ProgressManager::getInstance().setHighscore(i, "name", " ");
						ProgressManager::getInstance().setHighscore(i, "score", "0");
					}
					delete highscores;
					ProgressManager::getInstance().save();
					changeState(game, getInstance());
					break;
				}
				case 1: // Cancel
					break;
				default:
					SDL_Log("selection was %s");
					break;
				}
			});

			// Load header text
			loadText("highscore_title", "Highscores", "trebucbd", 48);

			_highscores = ProgressManager::getInstance().getHighscores();
			for (size_t i = 0, ilen = _highscores->size(); i < ilen; i++) {
				loadText(("number_" + std::to_string(i)), _highscores->at(i).at(0) + ".", "trebucbd", 36);
				loadText(("name_" + std::to_string(i)), _highscores->at(i).at(1), "trebucbd", 36);
				loadText(("score_" + std::to_string(i)), _highscores->at(i).at(2) == "0" ? " " : _highscores->at(i).at(2), "trebucbd", 36);
			}
			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void HighScoreState::cleanup(GameBase &game)
		{
			delete _menu;
			_menu = nullptr;
			
			game.getEngine()->getDrawEngine()->unload("highscore_background");
			game.getEngine()->getDrawEngine()->unload("highscore_title");

			for (size_t i = 0, ilen = _highscores->size(); i < ilen; i++) {
				game.getEngine()->getDrawEngine()->unload("number_" + std::to_string(i));
				game.getEngine()->getDrawEngine()->unload("name_" + std::to_string(i));
				game.getEngine()->getDrawEngine()->unload("score_" + std::to_string(i));
			}
			_highscores->clear();
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
				case SDL_CONTROLLERBUTTONDOWN:
					switch (event.cbutton.button)
					{
					case SDL_CONTROLLER_BUTTON_A:
						_menu->doAction();
						break;
					case SDL_CONTROLLER_BUTTON_B:
						_game->getStateManager()->popState();
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_UP:
						_menu->selectPrevious();
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
						_menu->selectNext();
						break;
					}
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_UP:
						_menu->selectPrevious();
						break;
					case SDLK_DOWN:
						_menu->selectNext();
						break;
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

			for (size_t i = 0, ilen = _highscores->size(); i < ilen; i++) {
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