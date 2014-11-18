#include "ControlsState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\input\InputEngine.h"
#include "gamestates\MainMenuState.h"
#include "gamestates\OptionsState.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"
#include "lib\JSONParser.h"
#include <iostream>
#include <fstream>
#include "helperclasses\ConfigManager.h"

namespace sdmg {
	namespace gamestates {

		void ControlsState::init(GameBase &game)
		{
			_game = &game;

			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - 187.5f, 100, game);
			std::function<void()> callback = std::bind(&ControlsState::menuAction, this);

			_menu->addMenuTextItem("Walk Right", callback);
			_menu->addMenuTextItem("Walk Left", callback);
			_menu->addMenuTextItem("Jump", callback);
			_menu->addMenuTextItem("Roll", callback);
			_menu->addMenuTextItem("Attack", callback);


			_walkright = new std::string("Right");
			_walkleft = new std::string("Left");
			_jump = new std::string("Up");
			_roll = new std::string("Numpad 0");
			_midrange = new std::string("1");

			_game->getEngine()->getDrawEngine()->loadDynamicText("info", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("player", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("walkright", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("walkleft", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("jump", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("roll", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("midrange", { 255, 255, 255 }, "trebucbd", 36);

			game.getEngine()->getDrawEngine()->load("controls_background", "assets/screens/mainbackground");

			_currentplayer = 0;
			readKeys();
		}

		void ControlsState::readKeys()
		{
			helperclasses::ConfigManager::getInstance();
			*_walkright = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "walkRight"));
			*_walkleft = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "walkLeft"));
			*_jump = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "jump"));
			*_roll = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "roll"));
			*_midrange = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "midrange"));
		}

		void ControlsState::menuAction()
		{

		}

		void ControlsState::cleanup(GameBase &game)
		{
			delete _menu;
			game.getEngine()->getDrawEngine()->unload("controls_background");
			game.getEngine()->getDrawEngine()->unload("info");
			game.getEngine()->getDrawEngine()->unload("player");
			game.getEngine()->getDrawEngine()->unload("walkright");
			game.getEngine()->getDrawEngine()->unload("walkleft");
			game.getEngine()->getDrawEngine()->unload("jump");
			game.getEngine()->getDrawEngine()->unload("roll");
			game.getEngine()->getDrawEngine()->unload("midrange");
			delete _walkright;
			delete _walkleft;
			delete _jump;
			delete _roll;
			delete _midrange;
			//game.getEngine()->getDrawEngine()->unloadAll();
			game.getEngine()->getInputEngine()->clearBindings();
		}

		void ControlsState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			if (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					game.stop();
				}

				if (event.type == SDL_KEYDOWN)
				{
					DrawEngine *drawEngine = _game->getEngine()->getDrawEngine();

					if (!_waiting)
					{
						switch (event.key.keysym.sym)
						{
							case SDLK_ESCAPE:
								_game->getStateManager()->popState();
								break;
							case SDLK_1:
								std::cout << "Key 1 pressed. Switching State.. " << std::endl;
								break;
							case SDLK_DOWN:
								_menu->selectNext();
								break;
							case SDLK_UP:
								_menu->selectPrevious();
								break;
							case SDLK_RIGHT:
								if (_currentplayer != 1)
									_currentplayer++;
								readKeys();
								break;
							case SDLK_LEFT:
								if (_currentplayer != 0)
									_currentplayer--;
								readKeys();
								break;
							case SDLK_KP_ENTER:
							case SDLK_RETURN:
								changeText(event.key.keysym.sym);
								_waiting = true;
								menuAction();
								break;
						}
					}
					else
					{
						InputEngine().findJoysticks();
						changeText(event.key.keysym.sym);
						bindKey();
					}
				}
			}
		}

		void ControlsState::changeText(const int keyCode)
		{
			std::string item = _menu->getSelectedMenuItem()->getTag();
			std::string text = "Press a key";

			if (_waiting) text = SDL_GetKeyName(keyCode);

			if (item == "Walk Right")
			{
				*_walkright = text;
			}
			else if (item == "Walk Left")
			{
				*_walkleft = text;
			}
			else if (item == "Jump")
			{
				*_jump = text;
			}
			else if (item == "Roll")
			{
				*_roll = text;
			}
			else if (item == "Attack")
			{
				*_midrange = text;
			}
		}

		void ControlsState::bindKey()
		{
			helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "walkRight", SDL_GetKeyFromName(_walkright->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "walkLeft", SDL_GetKeyFromName(_walkleft->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "jump", SDL_GetKeyFromName(_jump->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "roll", SDL_GetKeyFromName(_roll->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "midrange", SDL_GetKeyFromName(_midrange->c_str()));
			helperclasses::ConfigManager::getInstance().save();
			_waiting = false;
		}

		void ControlsState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void ControlsState::draw(GameBase &game, GameTime &gameTime)
		{
			DrawEngine *drawEngine = _game->getEngine()->getDrawEngine();

			drawEngine->prepareForDraw();
			drawEngine->draw("controls_background");

			drawEngine->drawDynamicText("info", "Press Left or Right to navigate between players.", 250, 550);
			drawEngine->drawDynamicText("player", "Player: " + std::to_string(_currentplayer + 1), 250, 50);
			drawEngine->drawDynamicText("walkright", *_walkright, 850, 125);
			drawEngine->drawDynamicText("walkleft", *_walkleft, 850, 195);
			drawEngine->drawDynamicText("jump", *_jump, 850, 270);
			drawEngine->drawDynamicText("roll", *_roll, 850, 345);
			drawEngine->drawDynamicText("midrange", *_midrange, 850, 415);

			_menu->draw(&game);

			drawEngine->render();
		}
	}
}