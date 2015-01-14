//
//
//  @ Project : SDMG
//  @ File Name : LevelSelectionState.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "LevelSelectionState.h"
#include "CharacterSelectionState.h"
#include "engine\GameTime.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"
#include "engine\input\InputEngine.h"
#include "engine\audio\AudioEngine.h"
#include "LoadingState.h"
#include "LoadingSinglePlayerState.h"
#include "MainMenuState.h"
#include "engine\util\FileManager.h"
#include "CreateLevelState.h"
#include "helperclasses\ProgressManager.h"

#include <vector>
#include <fstream>

namespace sdmg {
	namespace gamestates {

		void LevelSelectionState::init(GameBase &game)
		{
			_game = &game;
			_game->getEngine()->getDrawEngine()->loadText("levelselecttitle", game.getGameMode() == GameBase::GameMode::Edit ? "Select a level to edit" : "Select a level", { 255, 255, 255 }, "trebucbd", 48);
			
			// init menu
			_menu = new Menu(895, 540, game);
			_menu->addMenuTextItem(game.getGameMode() == GameBase::GameMode::Versus || game.getGameMode() == GameBase::GameMode::Edit ? "Select" : "Play", (std::function<void()>)std::bind(&LevelSelectionState::nextState, this));
			if (game.getGameMode() == GameBase::GameMode::Edit)
				_menu->addMenuTextItem("Back to main menu", (std::function<void()>)[&] { changeState(game, MainMenuState::getInstance()); });
			else
				_menu->addMenuTextItem("Back to characters", (std::function<void()>)[&] { changeState(game, CharacterSelectionState::getInstance()); });

			_levels = new std::vector<std::string>(util::FileManager::getInstance().getFiles("assets/levels/"));

			for (size_t i = 0; i < _levels->size(); i++)
			{
				const std::string levelFolder = (*_levels)[i];

				std::ifstream ifile("assets/levels/" + levelFolder + "/data");

				if (!ifile.fail())
				{
					JSON::JSONDocument *doc = nullptr;
					try
					{
						if (levelFolder != "tutorial"){
							doc = JSON::JSONDocument::fromFile("assets/levels/" + levelFolder + "/data");
							JSON::JSONObject &obj = doc->getRootObject();

							if (game.getGameMode() != GameBase::GameMode::Edit || obj.getBoolean("editable")) {

								bool showLevel = true;
								if (ProgressManager::getInstance().isUnlockableLevel(levelFolder))
									showLevel = ProgressManager::getInstance().isUnlockedLevel(levelFolder);

								if (showLevel)
								{
									game.getEngine()->getDrawEngine()->load(levelFolder + "_preview", "assets/levels/" + levelFolder + "/preview_big");
									std::string h = obj.getString("name");
									game.getEngine()->getDrawEngine()->loadText(levelFolder + "_title", obj.getString("name"), { 255, 255, 255 }, "trebucbd", 48);
								}
								else
								{
									_levels->erase(std::remove(_levels->begin(), _levels->end(), levelFolder), _levels->end());
									i--;
								}
							}
							else {
								_levels->erase(std::remove(_levels->begin(), _levels->end(), levelFolder), _levels->end());
								i--;
							}
						}
						else {
							_levels->erase(std::remove(_levels->begin(), _levels->end(), levelFolder), _levels->end());
							i--;
						}
					}
					catch (JSON::JSONException &ex)
					{
						std::cout << "LevelSelection: Error bij laden " + levelFolder;
					}
					catch(...)
					{
						std::cout << "LevelSelection: Error bij laden " + levelFolder;
					}

					delete doc;
				}
			}

			if (game.getGameMode() == GameBase::GameMode::Edit) {
				_levels->push_back("$newLevel");
				game.getEngine()->getDrawEngine()->load("$newLevel_preview", "assets/screens/level_new");
				game.getEngine()->getDrawEngine()->loadText("$newLevel_title", "Create new level", { 255, 255, 255 }, "trebucbd", 48);
			}

			game.getEngine()->getDrawEngine()->load("fade", "assets/screens/fadeout");
			game.getEngine()->getDrawEngine()->load("levelselect_background", "assets/screens/mainmenu");
			game.getEngine()->getInputEngine()->setMouseEnabled();

			auto windowWidth = game.getEngine()->getDrawEngine()->getWindowWidth();
			_xTargetPos = _xPos = _xStartPos = (windowWidth - PREVIEW_WIDTH) / 2;
			_xMinPos = _xStartPos - (_levels->size() - 1) * (PREVIEW_WIDTH + PREVIEW_PADDINGX);
			_currentLevel = 0;

			_step = 1.0f / 500.0f;
			_lastUpdate = std::chrono::high_resolution_clock::now();

			game.getEngine()->getInputEngine()->getMouse().setClickAction(0, PREVIEW_YPOS, _xStartPos, 315, (std::function<void()>)std::bind(&LevelSelectionState::selectPrevious, this));
			game.getEngine()->getInputEngine()->getMouse().setClickAction(_xStartPos + PREVIEW_WIDTH, PREVIEW_YPOS, windowWidth - _xStartPos - PREVIEW_WIDTH, 315, (std::function<void()>)std::bind(&LevelSelectionState::selectNext, this));
		}

		void LevelSelectionState::cleanup(GameBase &game)
		{
			delete _menu;
			game.getEngine()->getDrawEngine()->unloadAll();
			game.getEngine()->getInputEngine()->clearBindings();

			delete _levels;
			_levels = nullptr;
		}
		
		void LevelSelectionState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			if (SDL_PollEvent(&event))
			{
				game.getEngine()->getInputEngine()->handleEvent(event);
				
				if (event.type == SDL_QUIT)
				{
					cleanup(game);
					LoadingState::getInstance().cleanup(game);
					game.stop();
				}

				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						if (game.getGameMode() == GameBase::GameMode::Edit) 
							changeState(*_game, MainMenuState::getInstance());
						else
							changeState(*_game, CharacterSelectionState::getInstance());
						break;
					case SDLK_DOWN:
					case 1:
						_menu->selectNext();
						//game.getEngine()->getAudioEngine()->play("menu_switch_effect", 0);
						break;
					case SDLK_UP:
					case 0:
						_menu->selectPrevious();
						//game.getEngine()->getAudioEngine()->play("menu_switch_effect", 0);
						break;
					case SDLK_KP_ENTER:
					case SDLK_RETURN:
					case 10:
						_menu->doAction();
						break;

					case SDLK_LEFT:
						selectPrevious();
						break;
					case SDLK_RIGHT:
						selectNext();
						break;
					}
				}
				else if (event.type == SDL_CONTROLLERBUTTONDOWN)
				{
					switch (event.cbutton.button)
					{
					case SDL_CONTROLLER_BUTTON_B:
						changeState(*_game, CharacterSelectionState::getInstance());
						break;
					case SDL_CONTROLLER_BUTTON_A:
						_menu->doAction();
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_UP:
						_menu->selectPrevious();
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
						_menu->selectNext();
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
						selectPrevious();
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
						selectNext();
						break;
					}
				}
			}
		}

		void LevelSelectionState::update(GameBase &game, GameTime &gameTime)
		{
			auto curTime = std::chrono::high_resolution_clock::now();
			float diff = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - _lastUpdate).count() / 1000.0f;

			_lastUpdate = curTime;
			_accumulator += diff;

			while (_accumulator > _step) {
				if (_xPos < _xTargetPos) {
					_xPos += _scrollStep;
					if (_xPos > _xTargetPos)
						_xPos = _xTargetPos;
				}
				else if (_xPos > _xTargetPos) {
					_xPos -= _scrollStep;
					if (_xPos < _xTargetPos)
						_xPos = _xTargetPos;
				}
				else {
					_xPos = _xTargetPos;
					_scrollStep = SCROLL_STEP_SLOW;
				}

				_accumulator -= _step;
			}
		}

		void LevelSelectionState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("levelselect_background");
			game.getEngine()->getDrawEngine()->drawText("levelselecttitle", 50, 70);
			drawLevel(game);
			game.getEngine()->getDrawEngine()->draw("fade", 0, 511);
			_menu->draw(&game);



			game.getEngine()->getDrawEngine()->render();
		}

		void LevelSelectionState::nextState() {
			if (_game->getGameMode() == GameBase::GameMode::Edit && (*_levels)[_currentLevel] == "$newLevel") {
				changeState(*_game, CreateLevelState::getInstance());
			}
			else {
				LoadingState::getInstance().setIsTutorial(false);
				LoadingState::getInstance().setLevel(new std::string((*_levels)[_currentLevel]));
				changeState(*_game, LoadingState::getInstance());
			}
		}

		void LevelSelectionState::selectNext() {

			if (_xTargetPos > _xMinPos)
				_xTargetPos -= PREVIEW_WIDTH + PREVIEW_PADDINGX;
			else {
				_xTargetPos = _xStartPos;
				_scrollStep = SCROLL_STEP_QUICK;
			}

			if (_currentLevel < _levels->size() - 1) _currentLevel++;
			else _currentLevel = 0;
		}

		void LevelSelectionState::selectPrevious() {
			if (_xTargetPos < _xStartPos)
				_xTargetPos += PREVIEW_WIDTH + PREVIEW_PADDINGX;
			else
			{
				_xTargetPos = _xMinPos;
				_scrollStep = SCROLL_STEP_QUICK;
			}

			if (_currentLevel > 0) _currentLevel--;
			else _currentLevel = _levels->size() - 1;
		}

		void LevelSelectionState::drawLevel(GameBase &game) {

			int x = _xPos;

			for (auto level : *_levels) {
				auto textSize = game.getEngine()->getDrawEngine()->getTextSize(level + "_title");

				game.getEngine()->getDrawEngine()->draw(level + "_preview", x, PREVIEW_YPOS);
				game.getEngine()->getDrawEngine()->drawText(level + "_title", x + (PREVIEW_WIDTH - textSize[0]) / 2, PREVIEW_YPOS + PREVIEW_HEIGHT + TEXT_YPADDING);

				x += PREVIEW_WIDTH + PREVIEW_PADDINGX;
			}
		}
	}
}
