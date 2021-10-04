#include "StatisticsState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\input\InputEngine.h"
#include "OptionsState.h"
#include "helperclasses\ProgressManager.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"
#include <json.hpp>

namespace sdmg
{
	namespace gamestates
	{

		void StatisticsState::init(GameBase& game)
		{
			_game = &game;

			_menu = new Menu(50, 600, game);

			_screenNumber = FIRST_SCREEN;
			_windowWidth = _game->getEngine()->getDrawEngine()->getWindowWidth();

			if (_backName)
			{
				_menu->addMenuTextItem("Back to " + *_backName, (std::function<void()>)[&]
				{
					_game->getStateManager()->popState();
				});
				delete _backName;
				_backName = nullptr;
			}
			else
				_menu->addMenuTextItem("Back", (std::function<void()>)[&]
			{
				_game->getStateManager()->popState();
			});

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
			auto& jsonCharacterStatistics = ProgressManager::getInstance().getStatistics();

			for (auto& jsonCharacter : jsonCharacterStatistics)
			{
				auto characterName = jsonCharacter["name"].get<std::string>();
				loadText(characterName + "name", characterName, "trebucbd", 34);

				loadText(characterName + "wins", std::to_string(jsonCharacter["wins"].get<int>()), "trebucbd", 34);
				loadText(characterName + "losses", std::to_string(jsonCharacter["losses"].get<int>()), "trebucbd", 34);

				auto unlockedStatus = ProgressManager::getInstance().isUnlockedCharacter(characterName) ? "Yes" : "No";
				loadText(characterName + "unlocked", unlockedStatus, "trebucbd", 34);
			}
			// Screen 1 - Characters - End

			// Screen 2 - Levels - Start
			// Load levels
			auto& jsonLevelStatistics = ProgressManager::getInstance().getLevels();

			for (auto& jsonLevel : jsonLevelStatistics)
			{
				auto levelName = jsonLevel["name"].get<std::string>();
				loadText(levelName + "name_level", levelName, "trebucbd", 34);

				auto unlockedStatus = ProgressManager::getInstance().isUnlockableLevel(levelName) ? "Yes" : "No";
				loadText(levelName + "unlocked_level", unlockedStatus, "trebuc", 34);
			}
			// Screen 2 - Levels - End

			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void StatisticsState::setBackName(std::string name)
		{
			delete _backName;
			_backName = new std::string(name);
		}

		void StatisticsState::cleanup(GameBase& game)
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
			auto& jsonCharacterStatistics = ProgressManager::getInstance().getStatistics();

			for (auto& jsonCharacter : jsonCharacterStatistics)
			{
				auto characterName = jsonCharacter["name"].get<std::string>();
				game.getEngine()->getDrawEngine()->unload(characterName + "name");
				game.getEngine()->getDrawEngine()->unload(characterName + "wins");
				game.getEngine()->getDrawEngine()->unload(characterName + "losses");
				game.getEngine()->getDrawEngine()->unload(characterName + "unlocked");
			}

			// Unload levels
			auto& jsonLevelStatistics = ProgressManager::getInstance().getLevels();

			for (auto& jsonLevel : jsonLevelStatistics)
			{
				auto levelName = jsonLevel["name"].get<std::string>();
				game.getEngine()->getDrawEngine()->unload(levelName + "name_level");
				game.getEngine()->getDrawEngine()->unload(levelName + "unlocked_level");
			}

			game.getEngine()->getInputEngine()->getMouse().clear();
		}

		void StatisticsState::handleEvents(GameBase& game, GameTime& gameTime)
		{
			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
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
						case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
							if (_screenNumber < LAST_SCREEN)
								++_screenNumber;
							break;
						case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
							if (_screenNumber > FIRST_SCREEN)
								--_screenNumber;
							break;
						case SDL_CONTROLLER_BUTTON_B:
						case SDL_CONTROLLER_BUTTON_A:
						case SDL_CONTROLLER_BUTTON_START:
							_menu->doAction();
							break;
					}
				}

				game.getEngine()->getInputEngine()->handleEvent(event);
			}
		}

		void StatisticsState::update(GameBase& game, GameTime& gameTime)
		{
		}

		void StatisticsState::draw(GameBase& game, GameTime& gameTime)
		{
			DrawEngine* drawEngine = _game->getEngine()->getDrawEngine();
			drawEngine->prepareForDraw();
			drawEngine->draw("achievements_background");

			// Load statistics and levels
			auto& jsonCharacterStatistics = ProgressManager::getInstance().getStatistics();
			auto& jsonLevelStatistics = ProgressManager::getInstance().getLevels();

			int vpos = 290;
			int winspos = 680;
			int lossespos = 900;
			int unlockedpos = 440;
			int bulletxpos = (_windowWidth / 2) - (((BULLET_SIZE * LAST_SCREEN) + (BULLET_MARGIN * LAST_SCREEN - 1)) / 2);

			// Draw bullets
			for (int i = 1; i <= LAST_SCREEN; ++i)
			{
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
			switch (_screenNumber)
			{
				case 1: // Screen 1 - Characters
					drawEngine->drawText("wins", winspos, 240);
					drawEngine->drawText("losses", lossespos, 240);

					winspos += 10;
					lossespos += 10;
					unlockedpos += 10;

					for (auto& jsonCharacter : jsonCharacterStatistics)
					{
						auto characterName = jsonCharacter["name"].get<std::string>();
						drawEngine->drawText(characterName + "name", 50, vpos);
						drawEngine->drawText(characterName + "wins", winspos, vpos);
						drawEngine->drawText(characterName + "losses", lossespos, vpos);
						drawEngine->drawText(characterName + "unlocked", unlockedpos, vpos);

						vpos += 48;
					}
					break;
				case 2: // Screen 2 - Levels
					drawEngine->drawText("unlocked", unlockedpos, 240);

					unlockedpos += 10;

					for (auto& jsonLevel : jsonLevelStatistics)
					{
						auto levelName = jsonLevel["name"].get<std::string>();
						drawEngine->drawText(levelName + "name_level", 50, vpos);
						drawEngine->drawText(levelName + "unlocked_level", unlockedpos, vpos);

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
