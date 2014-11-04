#include "StatisticsState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\input\InputEngine.h"
#include "OptionsState.h"
#include "helperclasses\statistics\Statistics.h"
#include "lib\JSONParser.h"

#include <fstream>

namespace sdmg {
	namespace gamestates {

		void StatisticsState::init(GameBase &game)
		{
			_game = &game;

			doc = JSON::JSONDocument::fromFile("assets/statistics/statistics.json");

			game.getEngine()->getDrawEngine()->load("statics_background", "assets/screens/mainbackground");


			// Load header text
			loadText("title", "Statistics", "trebucbd", 48);
			loadText("wins", "Wins", "trebucbd", 36);
			loadText("losses", "Losses", "trebucbd", 36);

			// Load statistics
			bool useJSON = true;
			if (useJSON) {
				JSON::JSONObject &statisticsObj = doc->getRootObject();

				JSON::JSONArray &characterArr = statisticsObj.getArray("characters");

				for (int i = 0; i < characterArr.size(); i++) {
					JSON::JSONObject &characterObj = characterArr.getObject(i);
					// Get character name
					std::string charname = "";
					for (auto c : characterObj.getString("name")) {
						if (c != ' ')
							charname += tolower(c);
						else break;
					}
					// Load character name text
					loadText(charname + "name", characterObj.getString("name"), "trebucbd", 36);

					// Set character statistics
					loadText(charname + "wins", std::to_string(characterObj.getInt("wins")), "trebucbd", 36);
					loadText(charname + "losses", std::to_string(characterObj.getInt("losses")), "trebucbd", 36);
				}
			}
			else {
				std::vector<std::vector<std::string>> statistics = Statistics::getInstance().load();

				// Set character names
				loadText("nivekname", "Nivek the Assassin", "trebucbd", 36);
				loadText("silencename", "Silence the Code Ninja", "trebucbd", 36);
				loadText("fiatname", "Fiat Panda", "trebucbd", 36);
				loadText("luckyname", "Always Lucky Cowboy", "trebucbd", 36);
				loadText("mindname", "Mind the Ice Mage", "trebucbd", 36);
				loadText("enriquename", "Enrique the Suit is Bananas", "trebucbd", 36);

				// Set character statistics
				for (auto s : statistics) {
					loadText(s.at(0) + "wins", s.at(1), "trebucbd", 36);
					loadText(s.at(0) + "losses", s.at(2), "trebucbd", 36);
				}
			}
		}

		void StatisticsState::cleanup(GameBase &game)
		{
			game.getEngine()->getDrawEngine()->unload("statics_background");
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

			delete doc;
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
			bool useJSON = true;
			if (useJSON)
			{
				JSON::JSONObject &statisticsObj = doc->getRootObject();

				JSON::JSONArray &characterArr = statisticsObj.getArray("characters");

				for (int i = 0; i < characterArr.size(); i++) {
					JSON::JSONObject &characterObj = characterArr.getObject(i);
					// Get character name
					std::string charname = "";
					for (auto c : characterObj.getString("name")) {
						if (c != ' ')
							charname += tolower(c);
						else break;
					}
					drawEngine->drawText(charname + "name", 100, vpos);
					drawEngine->drawText(charname + "wins", winspos, vpos);
					drawEngine->drawText(charname + "losses", lossespos, vpos);

					vpos += 48;
				}
			}
			else 
			{
				std::vector<std::vector<std::string>> statistics = Statistics::getInstance().load();

				for (auto s : statistics) {
					drawEngine->drawText(s.at(0) + "name", 100, vpos);
					drawEngine->drawText(s.at(0) + "wins", winspos, vpos);
					drawEngine->drawText(s.at(0) + "losses", lossespos, vpos);

					vpos += 48;
				}
			}

			drawEngine->render();
		}

		void StatisticsState::loadText(std::string key, std::string text, std::string fontName, int fontSize)
		{
			_game->getEngine()->getDrawEngine()->loadText(key, text, { 255, 255, 255 }, fontName, fontSize);
		}
	}
}
