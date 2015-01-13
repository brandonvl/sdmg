//
//
//  @ Project : SDMG
//  @ File Name : CharacterSelectionState.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#include "CharacterSelectionState.h"
#include "LevelSelectionState.h"
#include "engine\GameTime.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"
#include "engine\input\InputEngine.h"
#include "engine\audio\AudioEngine.h"
#include "LoadingState.h"
#include "LoadingSinglePlayerState.h"
#include "LoadingSurvivalState.h"
#include "MainMenuState.h"
#include "GameModeState.h"
#include "engine\util\FileManager.h"
#include <vector>
#include <fstream>
#include "helperclasses\ProgressManager.h"

namespace sdmg {
	namespace gamestates {
		void CharacterSelectionState::init(GameBase &game)
		{
			_game = &game;

			_slotKeyInput = new std::map<std::string, int>();
			_characters = new std::map<std::string, JSON::JSONDocument*>();
			_lockedCharacters = new std::map<std::string, JSON::JSONDocument*>();
			_slots = new std::vector<std::string>();
			_keys = new std::vector<std::string>();
			//  _slots->resize(game.getGameMode() == GameBase::GameMode::Versus ? 2 : 1);
			_slots->resize(game.getGameMode() == GameBase::GameMode::Versus ? 4 : 1);
			SELECTED_CHARACTER_BOX_WIDTH = (game.getEngine()->getDrawEngine()->getWindowWidth() - SELECTED_CHARACTER_BOX_PADDING * _slots->size() - SELECTED_CHARACTER_BOX_PADDING) / _slots->size();

			std::map<std::string, engine::input::InputEngine::InputType> map = _game->getEngine()->getInputEngine()->getDevices();
			for (auto it : map) 
				_keys->push_back(it.first);

			for (int i = 0; i < _slots->size(); i++) {
				int x = (SELECTED_CHARACTER_BOX_WIDTH + SELECTED_CHARACTER_BOX_PADDING) * i + SELECTED_CHARACTER_BOX_PADDING;
				game.getEngine()->getInputEngine()->getMouse().setClickAction(x, SELECTED_CHARACTER_BOX_YPOS, SELECTED_CHARACTER_BOX_WIDTH, SELECTED_CHARACTER_BOX_HEIGHT, (std::function<void()>)[&, i] { (*_slots)[i].clear(); });
				
				std::string key = "slot_key_" + std::to_string(i);
				game.getEngine()->getDrawEngine()->loadDynamicText(key, { 255, 255, 255 }, "trebucbd", 20);
				game.getEngine()->getInputEngine()->getMouse().setClickAction(x, SELECTED_CHARACTER_BOX_YPOS - 60, SELECTED_CHARACTER_BOX_WIDTH, SELECTED_CHARACTER_BOX_HEIGHT / 5, (std::function<void()>)[&, key] {
					if (_slotKeyInput->at(key) >= _keys->size() - 1) { _slotKeyInput->at(key) = 0; }
					else { _slotKeyInput->at(key)++; }
				});
			}

			std::string titleText = "Select character";
			if (_slots->size() != 1) titleText += "s";

			_game->getEngine()->getDrawEngine()->loadText("characterselecttitle", titleText, { 255, 255, 255 }, "trebucbd", 48);

			// init menu
			_menu = new Menu(895, SMALL_CHARACTER_BOX_YPOS, game);
			_menu->addMenuTextItem(game.getGameMode() == GameBase::GameMode::Versus ? "Select level" : "Play", (std::function<void()>)std::bind(&CharacterSelectionState::nextState, this));
			_menu->addMenuTextItem("Back to game modes", (std::function<void()>)[&] { changeState(game, GameModeState::getInstance()); });


			game.getEngine()->getDrawEngine()->loadText("locked", "locked", { 51, 51, 51 }, "trebucbd", 12);

			std::vector<std::string> characterList = std::vector<std::string>(util::FileManager::getInstance().getFolders("assets/characters/"));

			for (size_t i = 0; i < characterList.size(); i++)
			{
				const std::string characterFolder = characterList[i];

				std::ifstream ifile("assets/characters/" + characterFolder + "/data");

				if (!ifile.fail())
				{
					try
					{
						auto doc = JSON::JSONDocument::fromFile("assets/characters/" + characterFolder + "/data");

						if (ProgressManager::getInstance().isUnlockedCharacter(characterFolder)) {					
							
							_characters->insert({ characterFolder, doc });

							game.getEngine()->getDrawEngine()->load("s_" + characterFolder + "_head", "assets/characters/" + characterFolder + "/preview");
							game.getEngine()->getDrawEngine()->load("s_" + characterFolder + "_big", "assets/characters/" + characterFolder + "/preview_big");
							game.getEngine()->getDrawEngine()->loadText("s_" + characterFolder + "_name", doc->getRootObject().getString("name"), { 255, 255, 255 }, "trebucbd", 12);

							int x = (SMALL_CHARACTER_BOX_WIDTH + SMALL_CHARACTER_BOX_PADDING) * (_characters->size() - 1) + SMALL_CHARACTER_BOX_PADDING;

							game.getEngine()->getInputEngine()->getMouse().setHoverAction(x, SMALL_CHARACTER_BOX_YPOS, 104, 126, (std::function<void()>)[&, characterFolder] {
								delete _currentCharacter;
								_currentCharacter = new std::string(characterFolder);
							});
							game.getEngine()->getInputEngine()->getMouse().setClickAction(x, SMALL_CHARACTER_BOX_YPOS, 104, 126, (std::function<void()>)std::bind(&CharacterSelectionState::select, this));
						}
						else {
							game.getEngine()->getDrawEngine()->load("s_" + characterFolder + "_head", "assets/characters/" + characterFolder + "/preview_locked");
							_lockedCharacters->insert({ characterFolder, doc });
						}
					}
					catch (...)
					{
						std::cout << "LevelSelection: Error loading " + characterFolder;
					}
				}
			}

			game.getEngine()->getDrawEngine()->load("characterselect_background", "assets/screens/mainmenu");
			game.getEngine()->getInputEngine()->setMouseEnabled();

			_currentCharacter = new std::string(_characters->begin()->first);
		}

		void CharacterSelectionState::cleanup(GameBase &game)
		{
			delete _menu;
			delete _currentCharacter;
			_currentCharacter = nullptr;
			delete _slotKeyInput;
			_slotKeyInput = nullptr;
			delete _keys;
			_keys = nullptr;
			game.getEngine()->getDrawEngine()->unloadAll();

			game.getEngine()->getInputEngine()->clearBindings();

			for (auto it : *_characters) {
				delete it.second;
				it.second = nullptr;
			}
			delete _characters;
			_characters = nullptr;

			for (auto it : *_lockedCharacters) {
				delete it.second;
				it.second = nullptr;
			}
			delete _lockedCharacters;
			_lockedCharacters = nullptr;

			delete _slots;
			_slots = nullptr;
		}
		
		void CharacterSelectionState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			if (SDL_PollEvent(&event))
			{
				game.getEngine()->getInputEngine()->handleEvent(event);

				if (event.type == SDL_QUIT)
				{
					//cleanup(game);
					//game.stop();
				}

				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						changeState(*_game, GameModeState::getInstance());
						break;
					case SDLK_RIGHT:
						selectNext();
						break;
					case SDLK_LEFT:
						selectPrevious();
						break;
					case SDLK_SPACE:
						select();
						break;

					case SDLK_DOWN:
					case 1:
						_menu->selectNext();
						break;
					case SDLK_UP:
					case 0:
						_menu->selectPrevious();
						break;
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
						changeState(*_game, GameModeState::getInstance());
						break;
					case SDL_CONTROLLER_BUTTON_A:
						select();
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
						selectPrevious();
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
						selectNext();
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_UP:
						_menu->selectPrevious();
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
						_menu->selectNext();
						break;
					case SDL_CONTROLLER_BUTTON_START:
						_menu->doAction();
						break;
					}
				}
			}
		}

		void CharacterSelectionState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void CharacterSelectionState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("characterselect_background");
			game.getEngine()->getDrawEngine()->drawText("characterselecttitle", 50, 70);

			drawCharacters(game);
			drawSelectedCharacters(game);

			_menu->draw(_game);

			game.getEngine()->getDrawEngine()->render();
		}

		void CharacterSelectionState::drawCharacters(GameBase &game) {
			auto de = game.getEngine()->getDrawEngine();

			int curPos = SMALL_CHARACTER_BOX_PADDING;

			for (auto p : *_characters) {
				if (p.first == *_currentCharacter) de->drawRectangle(Rectangle(curPos, SMALL_CHARACTER_BOX_YPOS, 104, 126), 217, 13, 13);
				de->drawRectangle(Rectangle(curPos + 2, SMALL_CHARACTER_BOX_YPOS + 2, SMALL_CHARACTER_BOX_WIDTH, SMALL_CHARACTER_BOX_HEIGHT), 81, 167, 204);
				de->draw("s_" + p.first + "_head", curPos + 2, SMALL_CHARACTER_BOX_YPOS + 2);

				auto size = de->getTextSize("s_" + p.first + "_name");
				int textX = curPos + (104 - size[0]) / 2;

				de->drawRectangle(Rectangle(curPos + 2, SMALL_CHARACTER_BOX_YPOS + SMALL_CHARACTER_BOX_WIDTH, SMALL_CHARACTER_BOX_HEIGHT, 24), 53, 121, 151);
				de->drawText("s_" + p.first + "_name", textX, SMALL_CHARACTER_BOX_YPOS + 105);

				curPos += SMALL_CHARACTER_BOX_WIDTH + SMALL_CHARACTER_BOX_PADDING;
			}

			auto lockedSize = de->getTextSize("locked");

			for (auto p : *_lockedCharacters) {
				de->drawRectangle(Rectangle(curPos + 2, SMALL_CHARACTER_BOX_YPOS + 2, SMALL_CHARACTER_BOX_WIDTH, SMALL_CHARACTER_BOX_HEIGHT), 81, 167, 204);
				de->draw("s_" + p.first + "_head", curPos + 2, SMALL_CHARACTER_BOX_YPOS + 2);
								
				int textX = curPos + (104 - lockedSize[0]) / 2;

				de->drawRectangle(Rectangle(curPos + 2, SMALL_CHARACTER_BOX_YPOS + SMALL_CHARACTER_BOX_WIDTH, SMALL_CHARACTER_BOX_HEIGHT, 24), 53, 121, 151);
				de->drawText("locked", textX, SMALL_CHARACTER_BOX_YPOS + 105);

				curPos += SMALL_CHARACTER_BOX_WIDTH + SMALL_CHARACTER_BOX_PADDING;
			}

		}

		void CharacterSelectionState::drawSelectedCharacters(GameBase &game) {
			auto de = game.getEngine()->getDrawEngine();

			int xPos = SELECTED_CHARACTER_BOX_PADDING;
			int slotNumber = 0;

			for (auto slot : *_slots) {
				de->drawRectangle(Rectangle(xPos, SELECTED_CHARACTER_BOX_YPOS, SELECTED_CHARACTER_BOX_WIDTH, SELECTED_CHARACTER_BOX_HEIGHT), 81, 167, 204);

				if (!slot.empty()) {
					auto size = de->getImageSize("s_" + slot + "_big");
					int imgPosX = xPos + (SELECTED_CHARACTER_BOX_WIDTH - size[0]) / 2;
					int imgPosY = SELECTED_CHARACTER_BOX_YPOS + (SELECTED_CHARACTER_BOX_HEIGHT - size[1]) / 2;

					de->draw("s_" + slot + "_big", imgPosX, imgPosY);

					de->drawRectangle(Rectangle(xPos, imgPosY - 80, SELECTED_CHARACTER_BOX_WIDTH, SELECTED_CHARACTER_BOX_HEIGHT / 6), 81, 167, 204);
					std::string key = "slot_key_" + std::to_string(slotNumber);
					if (_slotKeyInput->find(key) == _slotKeyInput->end())
						_slotKeyInput->insert(std::make_pair(key, slotNumber));
					int xText = xPos + (SELECTED_CHARACTER_BOX_WIDTH / 2);
					int textWidth = game.getEngine()->getDrawEngine()->getDynamicTextWidth(key) / 2;
					game.getEngine()->getDrawEngine()->drawDynamicText(key, _keys->at(_slotKeyInput->at(key)), xText - textWidth, imgPosY - 70);
					
				}
				slotNumber++;
				xPos += SELECTED_CHARACTER_BOX_WIDTH + SELECTED_CHARACTER_BOX_PADDING;
			}
		}

		void CharacterSelectionState::selectNext() {
			auto it = _characters->find(*_currentCharacter);
			it++;

			delete _currentCharacter;
			if (it == _characters->end()) _currentCharacter = new std::string(_characters->begin()->first);
			else _currentCharacter = new std::string(it->first);
		}

		void CharacterSelectionState::selectPrevious() {
			auto it = _characters->find(*_currentCharacter);

			delete _currentCharacter;

			if (it == _characters->begin()) {
				it = _characters->end();
			}
			it--;
			_currentCharacter = new std::string(it->first);
		}

		void CharacterSelectionState::select() {
			
			if (_slots->size() > 1) {
				for (int i = 0; i < _slots->size(); i++) {
					if ((*_slots)[i].empty()) {
						(*_slots)[i] = *_currentCharacter;
						return;
					}
				}
			}
			else (*_slots)[0] = *_currentCharacter;
		}

		void CharacterSelectionState::nextState() {
			int numPlayerCount = std::count_if(_slots->begin(), _slots->end(), [](std::string slot) { return !slot.empty(); });

			if (numPlayerCount >= 1) {
				switch (_game->getGameMode()) {
				case GameBase::GameMode::Versus:
					if (numPlayerCount >= 2) {
						LoadingState::getInstance().resetCharacters();
						LoadingState::getInstance().setSlotKeyBinding(_slotKeyInput, _keys);

						for (auto &slot : *_slots) {
							if (!slot.empty())
								LoadingState::getInstance().addCharacter(slot);
						}
						changeState(*_game, LevelSelectionState::getInstance());
					}
					break;

				case GameBase::GameMode::SinglePlayer:
					LoadingSinglePlayerState::getInstance().setPlayerName((*_slots)[0]);
					changeState(*_game, LoadingSinglePlayerState::getInstance());
					LoadingSinglePlayerState::getInstance().loadNextFight();
					break;

				case GameBase::GameMode::Survival:
					LoadingSurvivalState::getInstance().setPlayerName((*_slots)[0]);
					changeState(*_game, LoadingSurvivalState::getInstance());
					break;

				}
			}
		}

		std::string CharacterSelectionState::getSlotKeyInput(int slot) {
			std::string key = "slot_key_" + std::to_string(slot);
			auto it = _slotKeyInput->find(key);
			if (it != _slotKeyInput->end())
				return _keys->at(it->second);
			else
				return "";
		}
	}
}
