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
#include "engine\util\FileManager.h"
#include <vector>
#include <fstream>

namespace sdmg {
	namespace gamestates {
		void CharacterSelectionState::init(GameBase &game)
		{
			_game = &game;

			

			_characters = new std::map<std::string, JSON::JSONDocument*>();
			_slots = new std::vector<std::string>();
			_slots->resize(game.getGameMode() == GameBase::GameMode::Versus ? 4 : 1);

			std::string titleText = "Select character";
			if (_slots->size() != 1) titleText += "s";

			_game->getEngine()->getDrawEngine()->loadText("characterselecttitle", titleText, { 255, 255, 255 }, "trebucbd", 48);

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
						_characters->insert({ characterFolder, doc });

						game.getEngine()->getDrawEngine()->load("s_" + characterFolder + "_head", "assets/characters/" + characterFolder + "/preview");
						game.getEngine()->getDrawEngine()->load("s_" + characterFolder + "_big", "assets/characters/" + characterFolder + "/preview_big");
						game.getEngine()->getDrawEngine()->loadText("s_" + characterFolder + "_name", doc->getRootObject().getString("name"), { 255, 255, 255 }, "trebucbd", 12);
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
			delete _currentCharacter;
			game.getEngine()->getDrawEngine()->unloadAll();

			game.getEngine()->getInputEngine()->getMouse().clear();

			for (auto it : *_characters) {
				delete it.second;
			}
			delete _characters;
			delete _slots;
		}
		
		void CharacterSelectionState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			if (SDL_PollEvent(&event))
			{
				game.getEngine()->getInputEngine()->handleEvent(event);

				if (event.type == SDL_QUIT)
				{
					game.stop();
				}

				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						changeState(*_game, MainMenuState::getInstance());
						break;
					case SDLK_UP:
					case SDLK_RIGHT:
					case 1:
						selectNext();
						break;
					case SDLK_DOWN:
					case SDLK_LEFT:
					case 0:
						selectPrevious();
						break;
					case SDLK_SPACE:
					case 10:
						select();
						break;
					case SDLK_KP_ENTER:
					case SDLK_RETURN:
						nextState();
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

			game.getEngine()->getDrawEngine()->render();
		}

		void CharacterSelectionState::drawCharacters(GameBase &game) {
			auto de = game.getEngine()->getDrawEngine();

			int curPos = 10;
			int yPos = 590;
			for (auto p : *_characters) {
				if (p.first == *_currentCharacter) de->drawRectangle(Rectangle(curPos, yPos, 104, 126), 217, 13, 13);
				de->drawRectangle(Rectangle(curPos + 2, yPos + 2, 100, 100), 81, 167, 204);
				de->draw("s_" + p.first + "_head", curPos + 2, yPos + 2);

				auto size = de->getTextSize("s_" + p.first + "_name");
				int textX = curPos + (104 - size[0]) / 2;

				de->drawRectangle(Rectangle(curPos + 2, yPos + 100, 100, 24), 53, 121, 151);
				de->drawText("s_" + p.first + "_name", textX, yPos + 105);

				curPos += 110;
			}

		}

		void CharacterSelectionState::drawSelectedCharacters(GameBase &game) {
			auto de = game.getEngine()->getDrawEngine();

			int padding = 10;
			int blockWidth = (de->getWindowWidth() - padding * _slots->size() - padding) / _slots->size();
			int blockHeight = 300;

			int curPos = 10;
			int yPos = 230;
			for (auto slot : *_slots) {
				de->drawRectangle(Rectangle(curPos, yPos, blockWidth, blockHeight), 81, 167, 204);

				if (!slot.empty()) {
					auto size = de->getImageSize("s_" + slot + "_big");
					int imgPosX = curPos + (blockWidth - size[0]) / 2;
					int imgPosY = yPos + (blockHeight - size[1]) / 2;

					de->draw("s_" + slot + "_big", imgPosX, imgPosY);
				}

				curPos += blockWidth + padding;
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
	}
}
