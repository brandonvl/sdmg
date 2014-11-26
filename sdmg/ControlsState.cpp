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
			_menu->addMenuTextItem("Midrange attack", callback);
			_menu->addMenuTextItem("Longrange attack", callback);
			_menu->addMenuTextItem("Block", callback);
			_menu->addMenuTextItem("Back", callback);

			_info = new std::string("Press Left or Right to navigate between players.");
			_info2 = new std::string("Press Enter to change the key for the selected action. The controls are automatically saved.");
			_walkright = new std::string("Right");
			_walkleft = new std::string("Left");
			_jump = new std::string("Up");
			_roll = new std::string("Numpad 0");
			_midrange = new std::string("1");
			_longrange = new std::string("2");
			_block = new std::string("P");

			_game->getEngine()->getDrawEngine()->loadDynamicText("info", { 255, 255, 255 }, "trebucbd", 20);
			_game->getEngine()->getDrawEngine()->loadDynamicText("info2", { 255, 255, 255 }, "trebucbd", 20);
			_game->getEngine()->getDrawEngine()->loadDynamicText("player", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("walkright", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("walkleft", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("jump", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("roll", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("midrange", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("longrange", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("block", { 255, 255, 255 }, "trebucbd", 36);

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
			*_longrange = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "longrange"));
			*_block = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "block"));
		}

		void ControlsState::menuAction()
		{

		}

		void ControlsState::cleanup(GameBase &game)
		{
			delete _menu;
			game.getEngine()->getDrawEngine()->unload("controls_background");
			game.getEngine()->getDrawEngine()->unload("info");
			game.getEngine()->getDrawEngine()->unload("info2");
			game.getEngine()->getDrawEngine()->unload("player");
			game.getEngine()->getDrawEngine()->unload("walkright");
			game.getEngine()->getDrawEngine()->unload("walkleft");
			game.getEngine()->getDrawEngine()->unload("jump");
			game.getEngine()->getDrawEngine()->unload("roll");
			game.getEngine()->getDrawEngine()->unload("midrange");
			game.getEngine()->getDrawEngine()->unload("longrange");
			game.getEngine()->getDrawEngine()->unload("block");
			delete _walkright;
			delete _walkleft;
			delete _jump;
			delete _roll;
			delete _midrange;
			delete _info;
			delete _info2;
			delete _longrange;
			delete _block;
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
					// DrawEngine *drawEngine = _game->getEngine()->getDrawEngine();

					if (!_waiting)
					{
						switch (event.key.keysym.sym)
						{
							case SDLK_ESCAPE:
								_game->getStateManager()->popState();
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
			else if (item == "AttackMidRange")
			{
				*_midrange = text;
			}
			else if (item == "AttackLongRange")
			{
				*_longrange = text;
			}
			else if (item == "Block")
			{
				*_block = text;
			}
			else if (item == "Save")
			{
				_game->getStateManager()->popState();
			}
		}

		void ControlsState::bindKey()
		{
			helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "walkRight", SDL_GetKeyFromName(_walkright->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "walkLeft", SDL_GetKeyFromName(_walkleft->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "jump", SDL_GetKeyFromName(_jump->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "roll", SDL_GetKeyFromName(_roll->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "midrange", SDL_GetKeyFromName(_midrange->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "longrange", SDL_GetKeyFromName(_longrange->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "block", SDL_GetKeyFromName(_block->c_str()));
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

			drawEngine->drawDynamicText("info", *_info, 425, 40);
			drawEngine->drawDynamicText("info2", *_info2, 250, 90);
			drawEngine->drawDynamicText("player", "Player: " + std::to_string(_currentplayer + 1), 250, 30);
			drawEngine->drawDynamicText("walkright", *_walkright, 850, 155);
			drawEngine->drawDynamicText("walkleft", *_walkleft, 850, 225);
			drawEngine->drawDynamicText("jump", *_jump, 850, 300);
			drawEngine->drawDynamicText("roll", *_roll, 850, 375);
			drawEngine->drawDynamicText("midrange", *_midrange, 850, 445);
			drawEngine->drawDynamicText("longrange", *_longrange, 850, 515);
			drawEngine->drawDynamicText("block", *_block, 850, 585);

			_menu->draw(&game);

			drawEngine->render();
		}
	}
}