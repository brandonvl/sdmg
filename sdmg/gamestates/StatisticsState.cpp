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

			_menu = new Menu(50, 600, game);

			_screenNumber = FIRST_SCREEN;
			_windowWidth = _game->getEngine()->getDrawEngine()->getWindowWidth();

			if (_backName)
			{
				_menu->addMenuTextItem("Back to " + *_backName, (std::function<void()>)[&] { _game->getStateManager()->popState(); });
				delete _backName;
				_backName = nullptr;
			}
			else
				_menu->addMenuTextItem("Back", (std::function<void()>)[&] { _game->getStateManager()->popState(); });

			game.getEngine()->getDrawEngine()->load("achievements_background", "assets/screens/mainmenu");
			game.getEngine()->getDrawEngine()->load("achievements_bullet_grey", "assets/screens/bulletlist/bullet_grey");
			game.getEngine()->getDrawEngine()->load("achievements_bullet_white", "assets/screens/bulletlist/bullet_white");

			// Load header text
			loadText("title", "Achievements", "trebucbd", 48);

			// Screen 1 - Characters - Start
			loadText("wins", "Wins", "trebucbd", 34);
			loadText("losses", "Losses", "trebucbd", 34);
			loadText("unlocked", "Unlocked", "trebucbd", 34);

			// Load statistics
			JSON::JSONArray &statistics = ProgressManager::getInstance().getStatistics();

			for (int i = 0; i < statistics.size(); i++) {
				JSON::JSONObject &characterObj = statistics.getObject(i);
				// Load character name text
				loadText(characterObj.getString("name") + "name", characterObj.getString("name"), "trebucbd", 34);

				// Set character statistics
				loadText(characterObj.getString("name") + "wins", std::to_string(characterObj.getInt("wins")), "trebuc", 34);
				loadText(characterObj.getString("name") + "losses", std::to_string(characterObj.getInt("losses")), "trebuc", 34);

				std::string unlocked = ProgressManager::getInstance().isUnlockedCharacter(characterObj.getString("name")) ? "Yes" : "No";
				loadText(characterObj.getString("name") + "unlocked", unlocked, "trebuc", 34);
			}
			// Screen 1 - Characters - End

			// Screen 2 - Levels - Start
			// Load levels
			JSON::JSONArray &levels = ProgressManager::getInstance().getLevels();

			for (int i = 0; i < levels.size(); i++) {
				JSON::JSONObject &levelObj = levels.getObject(i);
				// Load level name text
				loadText(levelObj.getString("name") + "name_level", levelObj.getString("name"), "trebucbd", 34);
				
				std::string unlocked = ProgressManager::getInstance().isUnlockedLevel(levelObj.getString("name")) ? "Yes" : "No";
				loadText(levelObj.getString("name") + "unlocked_level", unlocked, "trebuc", 34);
			}
			// Screen 2 - Levels - End

			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void StatisticsState::setBackName(std::string name)
		{
			delete _backName;
			_backName = new std::string(name);
		}

		void StatisticsState::cleanup(GameBase &game)
		{
			delete _menu;
			_menu = nullptr;

			game.getEngine()->getDrawEngine()->unload("achievements_background");
			game.getEngine()->getDrawEngine()->unload("achievements_bullet_grey");
			game.getEngine()->getDrawEngine()->unload("achievements_bullet_white");
			game.getEngine()->getDrawEngine()->unload("title");
			game.getEngine()->getDrawEngine()->unload("wins");
			game.getEngine()->getDrawEngine()->unload("losses");
			game.getEngine()->getDrawEngine()->unload("unlocked");

			// Unload statistics
			JSON::JSONArray &statistics = ProgressManager::getInstance().getStatistics();

			for (int i = 0; i < statistics.size(); i++) {
				JSON::JSONObject &characterObj = statistics.getObject(i);
				game.getEngine()->getDrawEngine()->unload(characterObj.getString("name") + "name");
				game.getEngine()->getDrawEngine()->unload(characterObj.getString("name") + "wins");
				game.getEngine()->getDrawEngine()->unload(characterObj.getString("name") + "losses");
				game.getEngine()->getDrawEngine()->unload(characterObj.getString("name") + "unlocked");
			}

			// Unload levels
			JSON::JSONArray &levels = ProgressManager::getInstance().getLevels();

			for (int i = 0; i < levels.size(); i++) {
				JSON::JSONObject &levelObj = levels.getObject(i);
				game.getEngine()->getDrawEngine()->unload(levelObj.getString("name") + "name_level");
				game.getEngine()->getDrawEngine()->unload(levelObj.getString("name") + "unlocked_level");
			}

			game.getEngine()->getInputEngine()->getMouse().clear();
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
					case SDLK_LEFT:
						if (_screenNumber > FIRST_SCREEN)
							--_screenNumber;
						break;
					case SDLK_RIGHT:
						if (_screenNumber < LAST_SCREEN)
							++_screenNumber;
						break;
					case SDLK_ESCAPE:
					case SDLK_KP_ENTER:
					case SDLK_RETURN:
					case 10:
						_menu->doAction();
						break;
					}
				}
				else if (event.type == SDL_CONTROLLERBUTTONDOWN)
				{
					switch (event.cbutton.button)
					{
					case SDL_CONTROLLER_BUTTON_B:
					case SDL_CONTROLLER_BUTTON_A:
					case SDL_CONTROLLER_BUTTON_START:
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
			drawEngine->draw("achievements_background");

			// Load statistics and levels
			JSON::JSONArray &statistics = ProgressManager::getInstance().getStatistics();
			JSON::JSONArray &levels = ProgressManager::getInstance().getLevels();
			
			int vpos = 290;
			int winspos = 680;
			int lossespos = 900;
			int unlockedpos = 440;
			int bulletxpos = (_windowWidth / 2) - (((BULLET_SIZE * LAST_SCREEN) + (BULLET_MARGIN * LAST_SCREEN - 1)) / 2);

			// Draw bullets
			for (int i = 1; i <= LAST_SCREEN; ++i) {
				if (i == _screenNumber)
					drawEngine->draw("achievements_bullet_white", bulletxpos, BULLET_YPOS);
				else 
					drawEngine->draw("achievements_bullet_grey", bulletxpos, BULLET_YPOS);

				bulletxpos += BULLET_SIZE;

				if (i != LAST_SCREEN)
					bulletxpos += BULLET_MARGIN;

			}

			// Draw headers
			drawEngine->drawText("title", 50, 70);
			drawEngine->drawText("unlocked", unlockedpos, 240);

			// Draw screen content
			switch (_screenNumber) {
			case 1: // Screen 1 - Characters
				drawEngine->drawText("wins", winspos, 240);
				drawEngine->drawText("losses", lossespos, 240);

				winspos += 10;
				lossespos += 10;
				unlockedpos += 10;


				for (int i = 0; i < statistics.size(); i++) {
					JSON::JSONObject &characterObj = statistics.getObject(i);
					drawEngine->drawText(characterObj.getString("name") + "name", 50, vpos);
					drawEngine->drawText(characterObj.getString("name") + "wins", winspos, vpos);
					drawEngine->drawText(characterObj.getString("name") + "losses", lossespos, vpos);
					drawEngine->drawText(characterObj.getString("name") + "unlocked", unlockedpos, vpos);

					vpos += 48;
				}
				break;
			case 2: // Screen 2 - Levels
				drawEngine->drawText("unlocked", unlockedpos, 240);
				
				unlockedpos += 10;

				for (int i = 0; i < levels.size(); ++i) {
					JSON::JSONObject &levelObj = levels.getObject(i);
					drawEngine->drawText(levelObj.getString("name") + "name_level", 50, vpos);
					drawEngine->drawText(levelObj.getString("name") + "unlocked_level", unlockedpos, vpos);

					vpos += 48;
				}
				break;
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
