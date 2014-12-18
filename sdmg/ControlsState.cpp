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

			_menu = new Menu(50, 250, game);

			_menu->addMenuTextItem("Walk Right", (std::function<void()>)[&] { waitFor(_walkright); });
			_menu->addMenuTextItem("Walk Left", (std::function<void()>)[&] { waitFor(_walkleft); });
			_menu->addMenuTextItem("Jump", (std::function<void()>)[&] { waitFor(_jump); });
			_menu->addMenuTextItem("Roll", (std::function<void()>)[&] { waitFor(_roll); });
			_menu->addMenuTextItem("Midrange attack", (std::function<void()>)[&] { waitFor(_midrange); });
			_menu->addMenuTextItem("Longrange attack", (std::function<void()>)[&] { waitFor(_longrange); });
			_menu->addMenuTextItem("Block", (std::function<void()>)[&] { waitFor(_block); });
			_menu->addMenuTextItem("Back to options", (std::function<void()>)[&] { _game->getStateManager()->popState(); });

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
			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void ControlsState::waitFor(std::string *cur) {
			*cur = "Press a key";
			_current = cur;
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
				if (_current == nullptr) {
					game.getEngine()->getInputEngine()->handleEvent(event);
				}

				if (event.type == SDL_QUIT)
				{
					game.stop();
				}

				if (event.type == SDL_KEYDOWN)
				{
					// DrawEngine *drawEngine = _game->getEngine()->getDrawEngine();

					if (_current == nullptr)
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
							case 10:
								_menu->doAction();
								break;
						}
					}
					else setKey(event.key.keysym.sym);
				}
			}
		}

		void ControlsState::setKey(int keyCode) {
			*_current = SDL_GetKeyName(keyCode);
			save();
			_current = nullptr;
		}

		void ControlsState::save()
		{
			helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "walkRight", SDL_GetKeyFromName(_walkright->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "walkLeft", SDL_GetKeyFromName(_walkleft->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "jump", SDL_GetKeyFromName(_jump->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "roll", SDL_GetKeyFromName(_roll->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "midrange", SDL_GetKeyFromName(_midrange->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "longrange", SDL_GetKeyFromName(_longrange->c_str()));
			helperclasses::ConfigManager::getInstance().setKey(_currentplayer, "block", SDL_GetKeyFromName(_block->c_str()));
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

			drawEngine->drawDynamicText("info", *_info, 250, 20);
			drawEngine->drawDynamicText("info2", *_info2, 250, 50);
			drawEngine->drawDynamicText("player", "Player: " + std::to_string(_currentplayer + 1), 50, 30);
			drawEngine->drawDynamicText("walkright", *_walkright, 850, 115); // 155
			drawEngine->drawDynamicText("walkleft", *_walkleft, 850, 185);
			drawEngine->drawDynamicText("jump", *_jump, 850, 260);
			drawEngine->drawDynamicText("roll", *_roll, 850, 335);
			drawEngine->drawDynamicText("midrange", *_midrange, 850, 405);
			drawEngine->drawDynamicText("longrange", *_longrange, 850, 475);
			drawEngine->drawDynamicText("block", *_block, 850, 545);

			_menu->draw(&game);

			drawEngine->render();
		}
	}
}