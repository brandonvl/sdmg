#include "HelpState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\input\InputEngine.h"
#include "MainMenuState.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"
#include <sstream>
#include <fstream>

namespace sdmg {
	namespace gamestates {

		void HelpState::init(GameBase &game)
		{
			_game = &game;
			_menu = new Menu(50, 600, game);
			
			_menu->addMenuTextItem("Back to options", (std::function<void()>)[&] {
				_game->getStateManager()->popState();
			});

			game.getEngine()->getDrawEngine()->load("help_background", "assets/screens/mainmenu");

			loadText("title", "Help", "trebucbd", 48);
			
			std::ifstream input_file("assets/help.txt");
			std::string line;

			_text = new std::vector<std::string>();
			while (std::getline(input_file, line)) { 
				_text->push_back(line);
			}

			//_text = split("To win the game try to push your opponent of the edge\n", '\n');
			for (size_t i = 0, ilen = _text->size(); i < ilen; ++i) {
				loadText("howtowin" + std::to_string(i), _text->at(i), "trebuc", 24);
			}
			/*loadText("howtowin1", "To win the game try to push your opponent of the edge", "trebuc", 36);
			loadText("howtowin2", "or into Bullet Bob.", "trebuc", 36);
			loadText("howtowin3", "Victory shall be yours, when your opponent runs out of", "trebuc", 36);
			loadText("howtowin4", "lives.", "trebuc", 36);*/

			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void HelpState::cleanup(GameBase &game)
		{
			delete _menu;
			_menu = nullptr;

			game.getEngine()->getDrawEngine()->unload("help_background");
			game.getEngine()->getDrawEngine()->unloadText("title");

			for (size_t i = 0, ilen = _text->size(); i < ilen; ++i)
				game.getEngine()->getDrawEngine()->unloadText("howtowin" + std::to_string(i));

			delete _text;
			_text = nullptr;
			
			game.getEngine()->getInputEngine()->getMouse().clear();
		}

		void HelpState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				game.getEngine()->getInputEngine()->handleEvent(event);

				/*if (event.type == SDL_QUIT)
					game.stop();
				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
					case SDLK_KP_ENTER:
					case SDLK_RETURN:
					case 10:
						_menu->doAction();
						break;
					}
				}
				else if (event.type == SDL_CONTROLLERBUTTONDOWN)
				{
					_menu->doAction();
				}*/

				switch (event.type) {
				case SDL_QUIT:
					game.stop();
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
					case SDLK_KP_ENTER:
					case SDLK_RETURN:
					case 10:
						_menu->doAction();
						break;
					default:
						break;
					}
					break;
				case SDL_CONTROLLERBUTTONDOWN:
					_menu->doAction();
					break;
				default:
					break;
				}
			}

		}

		void HelpState::update(GameBase &game, GameTime &gameTime)
		{
		}

		void HelpState::draw(GameBase &game, GameTime &gameTime)
		{
			DrawEngine *drawEngine = _game->getEngine()->getDrawEngine();

			drawEngine->prepareForDraw();
			drawEngine->draw("help_background");

			int xpos = 50;
			int vpos = 70;
			drawEngine->drawText("title", xpos, vpos);
			vpos = 200;
			for (size_t i = 0, ilen = _text->size(); i < ilen; ++i)
				drawEngine->drawText("howtowin" + std::to_string(i), xpos, vpos += 48);

			_menu->draw(_game);

			drawEngine->render();
		}

		void HelpState::loadText(std::string key, std::string text, std::string fontName, int fontSize)
		{
			_game->getEngine()->getDrawEngine()->loadText(key, text, { 255, 255, 255 }, fontName, fontSize);
		}

		/*std::vector<std::string> &HelpState::split(const std::string &s, char delim, std::vector<std::string> &elems) {
			std::stringstream ss(s);
			std::string item;
			while (std::getline(ss, item, delim)) {
				elems.push_back(item);
			}
			return elems;
		}


		std::vector<std::string> HelpState::split(const std::string &s, char delim) {
			std::vector<std::string> elems;
			split(s, delim, elems);
			return elems;
		}*/
	}
}