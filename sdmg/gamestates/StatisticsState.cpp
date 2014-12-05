#include "StatisticsState.h"
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

		void StatisticsState::init(GameBase &game)
		{
			_game = &game;

			//  _menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - 187.5f, game.getEngine()->getDrawEngine()->getWindowHeight() / 2, game);
			_menu = new Menu(100, 600, game);

			_menu->addMenuTextItem("Back", (std::function<void()>)[&] { _game->getStateManager()->popState(); });

			game.getEngine()->getDrawEngine()->load("statics_background", "assets/screens/mainbackground");

			// Load header text
			loadText("title", "Statistics", "trebucbd", 48);
			loadText("wins", "Wins", "trebucbd", 36);
			loadText("losses", "Losses", "trebucbd", 36);

			// Load statistics
			JSON::JSONArray &statistics = ProgressManager::getInstance().getStatistics();

			for (int i = 0; i < statistics.size(); i++) {
				JSON::JSONObject &characterObj = statistics.getObject(i);
				// Load character name text
				loadText(characterObj.getString("name") + "name", characterObj.getString("name"), "trebucbd", 36);

				// Set character statistics
				loadText(characterObj.getString("name") + "wins", std::to_string(characterObj.getInt("wins")), "trebucbd", 36);
				loadText(characterObj.getString("name") + "losses", std::to_string(characterObj.getInt("losses")), "trebucbd", 36);
			}
		}

		void StatisticsState::cleanup(GameBase &game)
		{
			delete _menu;
			_menu = nullptr;

			// Load statistics
			JSON::JSONArray &statistics = ProgressManager::getInstance().getStatistics();

			game.getEngine()->getDrawEngine()->unload("statics_background");
			game.getEngine()->getDrawEngine()->unload("title");
			game.getEngine()->getDrawEngine()->unload("wins");
			game.getEngine()->getDrawEngine()->unload("losses");

			for (int i = 0; i < statistics.size(); i++) {
				JSON::JSONObject &characterObj = statistics.getObject(i);
				game.getEngine()->getDrawEngine()->unload(characterObj.getString("name") + "name");
				game.getEngine()->getDrawEngine()->unload(characterObj.getString("name") + "wins");
				game.getEngine()->getDrawEngine()->unload(characterObj.getString("name") + "losses");
			}
		}

		void StatisticsState::handleEvents(GameBase &game, GameTime &gameTime)
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

		void StatisticsState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void StatisticsState::draw(GameBase &game, GameTime &gameTime)
		{

			DrawEngine *drawEngine = _game->getEngine()->getDrawEngine();

			drawEngine->prepareForDraw();
			drawEngine->draw("statics_background");

			drawEngine->drawText("title", 100, 100);

			int winspos = 680;
			int lossespos = 900;

			drawEngine->drawText("wins", winspos, 200);
			drawEngine->drawText("losses", lossespos, 200);

			int vpos = 248;
			winspos += 10;
			lossespos += 10;

			// Load statistics
			JSON::JSONArray &statistics = ProgressManager::getInstance().getStatistics();

			for (int i = 0; i < statistics.size(); i++) {
				JSON::JSONObject &characterObj = statistics.getObject(i);
				drawEngine->drawText(characterObj.getString("name") + "name", 100, vpos);
				drawEngine->drawText(characterObj.getString("name") + "wins", winspos, vpos);
				drawEngine->drawText(characterObj.getString("name") + "losses", lossespos, vpos);

				vpos += 48;
			}
			
			_menu->draw(_game);

			drawEngine->render();
		}

		void StatisticsState::loadText(std::string key, std::string text, std::string fontName, int fontSize)
		{
			_game->getEngine()->getDrawEngine()->loadText(key, text, { 255, 255, 255 }, fontName, fontSize);
		}
	}
}
