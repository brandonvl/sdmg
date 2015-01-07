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
#include "MainMenuState.h"
#include "engine\util\FileManager.h"
#include <vector>
#include <fstream>

namespace sdmg {
	namespace gamestates {
		void CharacterSelectionState::init(GameBase &game)
		{
			_game = &game;
			_game->getEngine()->getDrawEngine()->loadText("characterselecttitle", "Select a characters", { 255, 255, 255 }, "trebucbd", 48);
			_menu = new Menu(50, 250, game);

			_characters = new std::map<std::string, JSON::JSONDocument*>();
			_slots = new std::vector<std::string>();

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
						game.getEngine()->getDrawEngine()->load("s_" + characterFolder + "_big", "assets/characters/" + characterFolder + "/win");
					}
					catch (...)
					{
						std::cout << "LevelSelection: Error loading " + characterFolder;
					}
				}
			}

			game.getEngine()->getDrawEngine()->load("characterselect_background", "assets/screens/mainmenu");
			game.getEngine()->getInputEngine()->setMouseEnabled();

			_currentCharacter = new std::string("nivek");
		}

		void CharacterSelectionState::cleanup(GameBase &game)
		{
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

			game.getEngine()->getDrawEngine()->render();
		}

		void CharacterSelectionState::drawCharacters(GameBase &game) {
			auto de = game.getEngine()->getDrawEngine();

			int curPos = 10;
			int yPos = 610;
			for (auto p : *_characters) {
				if (p.first == *_currentCharacter) de->drawRectangle(Rectangle(curPos, yPos, 104, 104), 217, 13, 13);
				de->drawRectangle(Rectangle(curPos + 2, yPos + 2, 100, 100), 81, 167, 204);
				de->draw("s_" + p.first + "_head", curPos + 2, yPos + 2);

				curPos += 110;
			}

		}

		void CharacterSelectionState::drawSelectedCharacters(GameBase &game) {
			auto de = game.getEngine()->getDrawEngine();

			int curPos = 10;
			int yPos = 100;
			for (auto slot : *_slots) {
				de->drawRectangle(Rectangle(curPos + 2, yPos + 2, 100, 100), 81, 167, 204);

				curPos += 6;
			}
		}
	}
}
