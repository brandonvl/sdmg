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

			helperclasses::menuitems::MenuTextItem *attack = new helperclasses::menuitems::MenuTextItem("Attack", 0, 68, false);
			attack->loadText(_game, "midrange", "Attack", "trebucbd", 33);
			_menu->addMenuItem(attack);

			_walkright = new std::string("Right");
			_walkleft = new std::string("Left");
			_jump = new std::string("Up");
			_roll = new std::string("Numpad 0");
			_midrange = new std::string("1");
			readKeys(1);

			_game->getEngine()->getDrawEngine()->loadDynamicText("info", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("player", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("walkright", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("walkleft", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("jump", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("roll", { 255, 255, 255 }, "trebucbd", 36);
			_game->getEngine()->getDrawEngine()->loadDynamicText("midrange", { 255, 255, 255 }, "trebucbd", 36);

			game.getEngine()->getDrawEngine()->load("controls", R"(assets\screens\help)");
		}

		void ControlsState::readKeys(int character)
		{
			std::ifstream keybindings;
			keybindings.close();
			if (character == 1)
				keybindings.open("assets\\controls\\player1");
			if (character == 2)
				keybindings.open("assets\\controls\\player2");

			if (keybindings.is_open()) {
				while (!keybindings.eof()) {
					std::string line;
					getline(keybindings, line);

					std::vector < std::string > tokens;
					std::string  temp;

					while (line.find("=", 0) != std::string::npos)
					{
						size_t  pos = line.find("=", 0);
						temp = line.substr(0, pos);
						line.erase(0, pos + 1);
						tokens.push_back(temp);
					}

					tokens.push_back(line);

					std::string name = tokens[0];
					int key = atoi(tokens[1].c_str());

					if (name == "Walk Right")
						*_walkright = SDL_GetKeyName(key);
					else if (name == "Walk Left")
						*_walkleft = SDL_GetKeyName(key);
					else if (name == "Jump")
						*_jump = SDL_GetKeyName(key);
					else if (name == "Roll")
						*_roll = SDL_GetKeyName(key);
					else if (name == "Attack")
						*_midrange = SDL_GetKeyName(key);
				}
			}
			
		}

		void ControlsState::menuAction(MenuItem *item)
		{

		}

		void ControlsState::cleanup(GameBase &game)
		{
			delete _menu;
			game.getEngine()->getDrawEngine()->unload("info");
			game.getEngine()->getDrawEngine()->unload("player");
			game.getEngine()->getDrawEngine()->unload("walkright");
			game.getEngine()->getDrawEngine()->unload("walkleft");
			game.getEngine()->getDrawEngine()->unload("jump");
			game.getEngine()->getDrawEngine()->unload("roll");
			game.getEngine()->getDrawEngine()->unload("midrange");
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
					DrawEngine *drawEngine = _game->getEngine()->getDrawEngine();

					if (!waiting)
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
								if (currentplayer != 2)
									currentplayer++;
								readKeys(currentplayer);
								break;
							case SDLK_LEFT:
								if (currentplayer != 1)
									currentplayer--;
								readKeys(currentplayer);
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
			else if (item == "Attack")
			{
				*_midrange = text;
			}
		}

		void ControlsState::bindKey()
		{
			std::ofstream keybindings;
			
			if (currentplayer == 1)
					keybindings.open("assets\\controls\\player1");
			else if (currentplayer == 2)
					keybindings.open("assets\\controls\\player2");


			std::string a_walkright = *_walkright;
			std::string a_walkleft = *_walkleft;
			std::string a_jump = *_jump;
			std::string a_roll = *_roll;
			std::string a_midrange = *_midrange;

			keybindings << "Walk Right=" + std::to_string(SDL_GetKeyFromName(a_walkright.c_str())) +"\n";
			keybindings << "Walk Left=" + std::to_string(SDL_GetKeyFromName(a_walkleft.c_str())) + "\n";
			keybindings << "Jump=" + std::to_string(SDL_GetKeyFromName(a_jump.c_str())) + "\n";
			keybindings << "Roll=" + std::to_string(SDL_GetKeyFromName(a_roll.c_str())) + "\n";
			keybindings << "Attack=" + std::to_string(SDL_GetKeyFromName(a_midrange.c_str()));
			keybindings.close();
			waiting = false;
		}

		void ControlsState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void ControlsState::draw(GameBase &game, GameTime &gameTime)
		{
			DrawEngine *drawEngine = _game->getEngine()->getDrawEngine();

			drawEngine->prepareForDraw();
			drawEngine->draw("controls");

			drawEngine->drawDynamicText("info", "Press Left or Right to navigate between players.", 250, 550);
			drawEngine->drawDynamicText("player", "Player: " + std::to_string(currentplayer), 250, 50);
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