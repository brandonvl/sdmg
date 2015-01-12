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

			_menu = new Menu(250, 125, game);

			_menu->addMenuTextItem("Walk Right", (std::function<void()>)[&] { waitFor(_walkright); });
			_menu->addMenuTextItem("Walk Left", (std::function<void()>)[&] { waitFor(_walkleft); });
			_menu->addMenuTextItem("Jump", (std::function<void()>)[&] { waitFor(_jump); });
			_menu->addMenuTextItem("Roll", (std::function<void()>)[&] { waitFor(_roll); });
			_menu->addMenuTextItem("Midrange attack", (std::function<void()>)[&] { waitFor(_midrange); });
			_menu->addMenuTextItem("Longrange attack", (std::function<void()>)[&] { waitFor(_longrange); });
			_menu->addMenuTextItem("Block", (std::function<void()>)[&] { waitFor(_block); });
			_menu->addMenuTextItem("Back to options", (std::function<void()>)[&] { _game->getStateManager()->popState(); });

			_info = new std::string("Press Left or Right to navigate between devices.");
			_info2 = new std::string("Press Enter to change the key for the selected action. The controls are automatically saved.");
			_devicename = new std::string("keyboardLEFT");
			_walkright = new std::string("Right");
			_walkleft = new std::string("Left");
			_jump = new std::string("Up");
			_roll = new std::string("Numpad 0");
			_midrange = new std::string("1");
			_longrange = new std::string("2");
			_block = new std::string("P");

			_keys = new std::vector<std::string>;

			_game->getEngine()->getDrawEngine()->loadText("controlstitle", "Controls", { 255, 255, 255 }, "trebucbd", 48);
			_game->getEngine()->getDrawEngine()->loadDynamicText("info", { 255, 255, 255 }, "trebucbd", 20);
			_game->getEngine()->getDrawEngine()->loadDynamicText("info2", { 255, 255, 255 }, "trebucbd", 20);
			_game->getEngine()->getDrawEngine()->loadDynamicText("devicename", { 255, 255, 255 }, "trebucbd", 20);
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
			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void ControlsState::waitFor(std::string *cur) {
			*cur = "Press a key";
			_current = cur;
		}

		void ControlsState::readKeys()
		{
			helperclasses::ConfigManager::getInstance();
			*_devicename = helperclasses::ConfigManager::getInstance().getDeviceName(_currentplayer);
			if (*_devicename == "keyboardRIGHT" || *_devicename == "keyboardLEFT")
			{
				*_walkright = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "walkRight"));
				*_walkleft = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "walkLeft"));
				*_jump = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "jump"));
				*_roll = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "roll"));
				*_midrange = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "midrange"));
				*_longrange = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "longrange"));
				*_block = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "block"));
			}
			else
			{
				*_walkright = SDL_GameControllerGetStringForButton(static_cast <SDL_GameControllerButton> (helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "walkRight")));
				*_walkleft = SDL_GameControllerGetStringForButton(static_cast <SDL_GameControllerButton> (helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "walkLeft")));
				*_jump = SDL_GameControllerGetStringForButton(static_cast <SDL_GameControllerButton> (helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "jump")));
				*_roll = SDL_GameControllerGetStringForButton(static_cast <SDL_GameControllerButton> (helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "roll")));
				*_midrange = SDL_GameControllerGetStringForButton(static_cast <SDL_GameControllerButton> (helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "midrange")));
				*_longrange = SDL_GameControllerGetStringForButton(static_cast <SDL_GameControllerButton> (helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "longrange")));
				*_block = SDL_GameControllerGetStringForButton(static_cast <SDL_GameControllerButton> (helperclasses::ConfigManager::getInstance().getKey(_currentplayer, "block")));
			}

			setKeys();
		}
		
		void ControlsState::cleanup(GameBase &game)
		{
			delete _menu;
			game.getEngine()->getDrawEngine()->unload("controls_background");
			game.getEngine()->getDrawEngine()->unloadText("controlstitle");
			game.getEngine()->getDrawEngine()->unload("info");
			game.getEngine()->getDrawEngine()->unload("info2");
			game.getEngine()->getDrawEngine()->unload("devicename");
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
			delete _devicename;
			delete _longrange;
			delete _block;

			delete _keys;
			//game.getEngine()->getDrawEngine()->unloadAll();
			game.getEngine()->getInputEngine()->clearBindings();
		}

		void ControlsState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			if (SDL_PollEvent(&event))
			{
				if (_current == nullptr) {
					game.getEngine()->getInputEngine()->handleEvent(event);
					//game.getEngine()->getInputEngine()->handleControllers(event);
				}

				if (event.type == SDL_QUIT)
				{
					game.stop();
				}

				if (event.type == SDL_KEYDOWN || event.type == SDL_CONTROLLERBUTTONDOWN)
				{
					if (_current == nullptr)
					{
						if (event.type == SDL_KEYDOWN)
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
									if (_currentplayer != 5)
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
										_menu->doAction();
									break;
							}
						}
						else if (event.type == SDL_CONTROLLERBUTTONDOWN)
						{
							switch (event.cbutton.button)
							{
							case SDL_CONTROLLER_BUTTON_B:
								_game->getStateManager()->popState();
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
							case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
								if (_currentplayer != 5)
									_currentplayer++;
								readKeys();
								break;
							case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
								if (_currentplayer != 0)
									_currentplayer--;
								readKeys();
								break;
							}
						}
					}
					else
					{
						if (*_devicename == "keyboardLEFT" || *_devicename == "keyboardRIGHT")
						{
							if (event.type == SDL_KEYDOWN && checkKey(event.key.keysym.sym))
							{
								setKey(SDL_GetKeyName(event.key.keysym.sym), *_devicename);
								setKeys();
							}
						}
						else
						{
							if (event.type == SDL_CONTROLLERBUTTONDOWN)
							{
								SDL_GameControllerButton btn = static_cast <SDL_GameControllerButton> (event.cbutton.button);
								setKey(SDL_GameControllerGetStringForButton(btn), *_devicename);
								setKeys();
							}
						}
					}
				}
			}
		}

		bool ControlsState::checkKey(int keyCode)
		{
			for (auto i : (*_keys)) {
				if (SDL_GetKeyName(keyCode) == i) return false;
			}

			return true;
		}

		void ControlsState::setKeys()
		{
			_keys->clear();
			_keys->push_back(*_walkright);
			_keys->push_back(*_walkleft);
			_keys->push_back(*_jump);
			_keys->push_back(*_roll);
			_keys->push_back(*_midrange);
			_keys->push_back(*_longrange);
			_keys->push_back(*_block);
		}

		void ControlsState::setKey(std::string keyCode, std::string deviceName) {
			if (deviceName == *_devicename)
			{
				*_current = keyCode;
				save(deviceName);
				_current = nullptr;
			}
			
		}

		void ControlsState::save(std::string deviceName)
		{
			if (deviceName == "keyboardRIGHT" || deviceName == "keyboardLEFT")
			{
				helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "walkRight", SDL_GetKeyFromName(_walkright->c_str()), deviceName);
				helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "walkLeft", SDL_GetKeyFromName(_walkleft->c_str()), deviceName);
				helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "jump", SDL_GetKeyFromName(_jump->c_str()), deviceName);
				helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "roll", SDL_GetKeyFromName(_roll->c_str()), deviceName);
				helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "midrange", SDL_GetKeyFromName(_midrange->c_str()), deviceName);
				helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "longrange", SDL_GetKeyFromName(_longrange->c_str()), deviceName);
				helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "block", SDL_GetKeyFromName(_block->c_str()), deviceName);
			}
			else
			{
				helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "walkRight", SDL_GameControllerGetButtonFromString(_walkright->c_str()), deviceName);
				helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "walkLeft", SDL_GameControllerGetButtonFromString(_walkleft->c_str()), deviceName);
				helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "jump", SDL_GameControllerGetButtonFromString(_jump->c_str()), deviceName);
				helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "roll", SDL_GameControllerGetButtonFromString(_roll->c_str()), deviceName);
				helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "midrange", SDL_GameControllerGetButtonFromString(_midrange->c_str()), deviceName);
				helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "longrange", SDL_GameControllerGetButtonFromString(_longrange->c_str()), deviceName);
				helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "block", SDL_GameControllerGetButtonFromString(_block->c_str()), deviceName);
			}
			helperclasses::ConfigManager::getInstance().save();			
		}

		void ControlsState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void ControlsState::draw(GameBase &game, GameTime &gameTime)
		{
			DrawEngine *drawEngine = _game->getEngine()->getDrawEngine();

			drawEngine->prepareForDraw();
			drawEngine->draw("controls_background");

			game.getEngine()->getDrawEngine()->drawText("controlstitle", 50, 70);
			drawEngine->drawDynamicText("info", *_info, 250, 20);
			drawEngine->drawDynamicText("info2", *_info2, 250, 50);
			drawEngine->drawDynamicText("devicename", "Device: " + *_devicename, 250, 90);
			drawEngine->drawDynamicText("walkright", *_walkright, 650, 145);
			drawEngine->drawDynamicText("walkleft", *_walkleft, 650, 215);
			drawEngine->drawDynamicText("jump", *_jump, 650, 290);
			drawEngine->drawDynamicText("roll", *_roll, 650, 365);
			drawEngine->drawDynamicText("midrange", *_midrange, 650, 435);
			drawEngine->drawDynamicText("longrange", *_longrange, 650, 505);
			drawEngine->drawDynamicText("block", *_block, 650, 575);

			_menu->draw(&game);

			drawEngine->render();
		}
	}
}