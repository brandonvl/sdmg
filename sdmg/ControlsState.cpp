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

			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - 187.5f, 100);

			// Create menu item
			helperclasses::menuitems::MenuTextItem *right = new helperclasses::menuitems::MenuTextItem("Walk Right", 0, 68, true);
			right->loadText(_game, "walkright", "Walk Right", "trebucbd", 33);
			_menu->addMenuItem(right);

			helperclasses::menuitems::MenuTextItem *left = new helperclasses::menuitems::MenuTextItem("Walk Left", 0, 68, false);
			left->loadText(_game, "walkleft", "Walk Left", "trebucbd", 33);
			_menu->addMenuItem(left);

			helperclasses::menuitems::MenuTextItem *jump = new helperclasses::menuitems::MenuTextItem("Jump", 0, 68, false);
			jump->loadText(_game, "jump", "Jump", "trebucbd", 33);
			_menu->addMenuItem(jump);

			helperclasses::menuitems::MenuTextItem *roll = new helperclasses::menuitems::MenuTextItem("Roll", 0, 68, false);
			roll->loadText(_game, "roll", "Roll", "trebucbd", 33);
			_menu->addMenuItem(roll);

			helperclasses::menuitems::MenuTextItem *attackMid = new helperclasses::menuitems::MenuTextItem("AttackMidRange", 0, 68, false);
			attackMid->loadText(_game, "midrange", "AttackMidRange", "trebucbd", 33);
			_menu->addMenuItem(attackMid);

			helperclasses::menuitems::MenuTextItem *attackLong = new helperclasses::menuitems::MenuTextItem("AttackLongRange", 0, 68, false);
			attackLong->loadText(_game, "longrange", "AttackLongRange", "trebucbd", 33);
			_menu->addMenuItem(attackLong);

			helperclasses::menuitems::MenuTextItem *block = new helperclasses::menuitems::MenuTextItem("Block", 0, 68, false);
			block->loadText(_game, "block", "Block", "trebucbd", 33);
			_menu->addMenuItem(block);

			_walkright = new std::string("Right");
			_walkleft = new std::string("Left");
			_jump = new std::string("Up");
			_roll = new std::string("Numpad 0");
			_midrange = new std::string("1");
			_longrange = new std::string("2");
			_block = new std::string("P");

			_game->getEngine()->getDrawEngine()->loadDynamicText("info", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("player", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("walkright", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("walkleft", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("jump", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("roll", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("midrange", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("longrange", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("block", { 255, 255, 255 }, "trebucbd", 36);

			game.getEngine()->getDrawEngine()->load("controls_background", "assets/screens/mainbackground");

			currentplayer = 0;
			readKeys();
		}

		void ControlsState::readKeys()
		{
			helperclasses::ConfigManager::getInstance();
			*_walkright = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(currentplayer, "walkRight"));
			*_walkleft = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(currentplayer, "walkLeft"));
			*_jump = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(currentplayer, "jump"));
			*_roll = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(currentplayer, "roll"));
			*_midrange = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(currentplayer, "midrange"));
			*_longrange = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(currentplayer, "longrange"));
			*_block = SDL_GetKeyName(helperclasses::ConfigManager::getInstance().getKey(currentplayer, "block"));
		}

		void ControlsState::menuAction(MenuItem *item)
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
			game.getEngine()->getDrawEngine()->unload("longrange");
			game.getEngine()->getDrawEngine()->unload("block");
			delete _walkright;
			delete _walkleft;
			delete _jump;
			delete _roll;
			delete _midrange;
			delete _longrange;
			delete _block;
			//game.getEngine()->getDrawEngine()->unloadAll();
			game.getEngine()->getInputEngine()->clearBindings();
		}

		void ControlsState::pause(GameBase &game)
		{
		}

		void ControlsState::resume(GameBase &game)
		{
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

					if (!waiting)
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
								if (currentplayer != 1)
									currentplayer++;
								readKeys();
								break;
							case SDLK_LEFT:
								if (currentplayer != 0)
									currentplayer--;
								readKeys();
								break;
							case SDLK_KP_ENTER:
							case SDLK_RETURN:
								changeText(waiting, event.key.keysym.sym);
								waiting = true;
								menuAction(_menu->getSelectedMenuItem());
								break;
						}
					}
					else
					{
						InputEngine().findJoysticks();
						changeText(waiting, event.key.keysym.sym);
						bindKey();
					}
				}
			}
		}

		void ControlsState::changeText(bool waiting, const int keyCode)
		{
			std::string item = _menu->getSelectedMenuItem()->getTag();
			std::string text = "Press a key";

			if (waiting) text = SDL_GetKeyName(keyCode);

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
		}

		void ControlsState::bindKey()
		{
			helperclasses::ConfigManager::getInstance().setKey(currentplayer, "walkRight", SDL_GetKeyFromName(_walkright->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(currentplayer, "walkLeft", SDL_GetKeyFromName(_walkleft->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(currentplayer, "jump", SDL_GetKeyFromName(_jump->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(currentplayer, "roll", SDL_GetKeyFromName(_roll->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(currentplayer, "midrange", SDL_GetKeyFromName(_midrange->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(currentplayer, "longrange", SDL_GetKeyFromName(_longrange->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(currentplayer, "block", SDL_GetKeyFromName(_block->c_str()));
			helperclasses::ConfigManager::getInstance().save();
			waiting = false;
		}

		void ControlsState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void ControlsState::draw(GameBase &game, GameTime &gameTime)
		{
			DrawEngine *drawEngine = _game->getEngine()->getDrawEngine();

			drawEngine->prepareForDraw();
			drawEngine->draw("controls_background");

			drawEngine->drawDynamicText("info", "Press Left or Right to navigate between players.", 250, 630);
			drawEngine->drawDynamicText("player", "Player: " + std::to_string(currentplayer + 1), 250, 50);
			drawEngine->drawDynamicText("walkright", *_walkright, 850, 125);
			drawEngine->drawDynamicText("walkleft", *_walkleft, 850, 195);
			drawEngine->drawDynamicText("jump", *_jump, 850, 270);
			drawEngine->drawDynamicText("roll", *_roll, 850, 345);
			drawEngine->drawDynamicText("midrange", *_midrange, 850, 415);
			drawEngine->drawDynamicText("longrange", *_longrange, 850, 485);
			drawEngine->drawDynamicText("block", *_block, 850, 555);

			_menu->draw(&game);

			drawEngine->render();
		}
	}
}