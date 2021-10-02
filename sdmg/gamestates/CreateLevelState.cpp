#include "CreateLevelState.h"
#include <SDL.h>
#include "engine\GameBase.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\input\InputEngine.h"
#include "helperclasses\Menu.h"
#include "LevelSelectionState.h"
#include "LoadingState.h"
#include <algorithm>
#include "engine\util\FileManager.h"

namespace sdmg {
	namespace gamestates {
		void CreateLevelState::init(GameBase &game) {
			_name = new std::string();
			_game = &game;
			_error = new std::string();

			game.getEngine()->getDrawEngine()->loadText("createtitle", "Enter a name for your new level", { 255, 255, 255 }, "trebucbd", 48);
			game.getEngine()->getDrawEngine()->loadText("existserr", "There already exists a level with this name.", { 217, 13, 13 }, "trebucbd", 20);
			game.getEngine()->getDrawEngine()->loadText("invaliderr", "Enter a name for your new level.", { 217, 13, 13 }, "trebucbd", 20);
			game.getEngine()->getDrawEngine()->loadDynamicText("input", { 255, 255, 255 }, "trebucbd", 48);
			game.getEngine()->getDrawEngine()->load("create_background", "assets/screens/mainmenu");

			//std::function<void(MenuItem *item)> callBack = &MainMenuState::menuAction;
			_menu = new Menu(453, 360, game);

			_menu->addMenuTextItem("Create", (std::function<void()>)std::bind(&CreateLevelState::create, this));
			_menu->addMenuTextItem("Cancel", (std::function<void()>)[&] { 
				_game->getStateManager()->pushState(LevelSelectionState::getInstance()); 
			});

			game.getEngine()->getInputEngine()->setMouseEnabled();
		}
		void CreateLevelState::cleanup(GameBase &game) {
			delete _name;
			delete _error;
			
			game.getEngine()->getDrawEngine()->unloadAll();
		}

		void CreateLevelState::pause(GameBase &game) {

		}
		void CreateLevelState::resume(GameBase &game) {

		}

		void CreateLevelState::handleEvents(GameBase &game, GameTime &gameTime) {

			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				game.getEngine()->getInputEngine()->handleEvent(event);

				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym) {
					case SDLK_BACKSPACE:
						if (_name->size() > 0)
							_name->pop_back();
						break;
					case SDLK_SPACE:
						*_name += " ";
						break;
					case SDLK_DOWN:
						_menu->selectNext();
						break;
					case SDLK_UP:
						_menu->selectPrevious();
						break;
					case SDLK_RETURN:
						_menu->doAction();
						break;
					default:
						if (_name->size() <= 18) {
							
							std::string input = SDL_GetKeyName(event.key.keysym.sym);
							if (input.size() == 1 && input[0] >= 'A' && input[0] <= 'Z') {
								*_name += _name->size() == 0 ? input[0] : tolower(input[0]);
							}

						}
						break;
					}
				}
			}

		}
		void CreateLevelState::update(GameBase &game, GameTime &gameTime) {

		}
		void CreateLevelState::draw(GameBase &game, GameTime &gameTime) {

			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("create_background");
			game.getEngine()->getDrawEngine()->drawText("createtitle", 50, 75);

			int x = (game.getEngine()->getDrawEngine()->getWindowWidth() - 600) / 2;
			game.getEngine()->getDrawEngine()->drawRectangle(Rectangle(x, 250, 600, 80), 102, 175, 207);
			game.getEngine()->getDrawEngine()->drawDynamicText("input", *_name, x + 20, 260);

			if (!_error->empty()) {
				auto size = game.getEngine()->getDrawEngine()->getTextSize(*_error);
				int errX = (game.getEngine()->getDrawEngine()->getWindowWidth() - size[0]) / 2;

				game.getEngine()->getDrawEngine()->drawText(*_error, errX, 210);
			}

			_menu->draw(&game);

			game.getEngine()->getDrawEngine()->render();

		}

		void CreateLevelState::create() {

			std::string pathName = *_name;
			std::string path = "assets/levels/" + pathName + "/";
			std::transform(pathName.begin(), pathName.end(), pathName.begin(), ::tolower);
			
			pathName.erase(remove_if(pathName.begin(), pathName.end(), isspace), pathName.end());
			// validate pathname
			if (pathName.size() == 0) {
				*_error = "invaliderr";
				return;
			}
			
			auto curLevels = util::FileManager::getInstance().getFolders("assets/levels/");			
			if (std::find(curLevels.begin(), curLevels.end(), pathName) != curLevels.end()) {
				*_error = "existserr";
				return;
			}

			util::FileManager::getInstance().createFolder(path);

			nlohmann::json levelContent;

			levelContent["name"] = *_name;
			levelContent["editable"] = true;

			levelContent["gravity"]["left"] = 0;
			levelContent["gravity"]["down"] = 100;

			levelContent["startingPositions"][0]["x"] = 150;
			levelContent["startingPositions"][0]["y"] = -100;

			levelContent["startingPositions"][1]["x"] = 450;
			levelContent["startingPositions"][1]["y"] = -100;

			levelContent["startingPositions"][2]["x"] = 750;
			levelContent["startingPositions"][2]["y"] = -100;

			levelContent["startingPositions"][3]["x"] = 1050;
			levelContent["startingPositions"][3]["y"] = -100;

			levelContent["platforms"] = nlohmann::json::array();

			util::FileManager::getInstance().saveJsonContentToFile(path + "data", levelContent);

			LoadingState::getInstance().setIsTutorial(false);
			LoadingState::getInstance().setLevel(new std::string(*_name));
			_game->getStateManager()->pushState(LoadingState::getInstance());
		}
	}
}