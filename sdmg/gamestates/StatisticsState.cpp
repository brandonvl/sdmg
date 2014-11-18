#include "StatisticsState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\input\InputEngine.h"
#include "OptionsState.h"
#include "lib\JSONParser.h"
#include "helperclasses\ProgressManager.h"

namespace sdmg {
	namespace gamestates {

		void StatisticsState::init(GameBase &game)
		{
			_game = &game;
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
				case SDL_MOUSEBUTTONDOWN:
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

			drawEngine->render();
		}

		void StatisticsState::loadText(std::string key, std::string text, std::string fontName, int fontSize)
		{
			_game->getEngine()->getDrawEngine()->loadText(key, text, { 255, 255, 255 }, fontName, fontSize);
		}
	}
}
