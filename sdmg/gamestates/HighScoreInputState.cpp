#include "HighScoreInputState.h"
#include "HighScoreState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\input\InputEngine.h"
#include "lib\JSONParser.h"
#include "helperclasses\ProgressManager.h"
#include "helperclasses\Menu.h"
#include <sstream>
#include <memory>

namespace sdmg {
	namespace gamestates {
		void HighScoreInputState::init(GameBase &game)
		{
			_game = &game;
			game.getEngine()->getDrawEngine()->load("highscoreinput_background", "assets/screens/mainmenu");
			
			// Create keyboard
			initKeyboard(game);

			// Load text
			loadText("highscoreinput_title", "Highscore: Input your Initials", "trebucbd", 48);
			//loadDynamicText("highscoreinput_initials",  255, 255, 255, "trebucbd", 72);
			_game->getEngine()->getDrawEngine()->loadDynamicText("highscoreinput_initials", { 255, 255, 255 }, "trebucbd", 160);

			_highscoreInitials = new std::string();

			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void HighScoreInputState::cleanup(GameBase &game)
		{
			if (!_keyboard->empty()) {
				for (auto i = 0; i < _keyboard->size(); ++i) {
					delete _keyboard->at(i);
					_keyboard->at(i) = nullptr;
				}
			}
			delete _keyboard;
			_keyboard = nullptr;

			game.getEngine()->getDrawEngine()->unload("highscoreinput_background");
			game.getEngine()->getDrawEngine()->unload("highscoreinput_title");
			game.getEngine()->getDrawEngine()->unload("highscoreinput_initials");

			delete _highscoreInitials;
			_highscoreInitials = nullptr;

			game.getEngine()->getInputEngine()->getMouse().clear();
		}

		void HighScoreInputState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				game.getEngine()->getInputEngine()->handleEvent(event);

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
						//_keyboard.at(0)->doAction();
						/*if (_highscoreInitials->size() < 3)
							_highscoreInitials->append("X");*/
						break;
					}
				}
			}
		}
		
		void HighScoreInputState::update(GameBase &game, GameTime &gameTime)
		{

		}

		void HighScoreInputState::draw(GameBase &game, GameTime &gameTime)
		{
			DrawEngine *drawEngine = game.getEngine()->getDrawEngine();
			drawEngine->prepareForDraw();
			drawEngine->draw("highscoreinput_background");
			drawEngine->drawText("highscoreinput_title", 50, 70);
			if (!_highscoreInitials->empty()) 
				drawEngine->drawDynamicText("highscoreinput_initials", *_highscoreInitials, 1020 - drawEngine->getDynamicTextWidth("highscoreinput_initials")/2, 300);

			for (auto keycol : *_keyboard)
				keycol->draw(_game);

			game.getEngine()->getDrawEngine()->render();
		}


		void HighScoreInputState::loadText(std::string key, std::string text, std::string fontName, int fontSize)
		{
			_game->getEngine()->getDrawEngine()->loadText(key, text, { 255, 255, 255 }, fontName, fontSize);
		}

		/*void HighScoreInputState::loadDynamicText(std::string key, int r, int g, int b, std::string fontName, int fontSize)
		{
			_game->getEngine()->getDrawEngine()->loadDynamicText(key, { 255, 255, 255 }, fontName, fontSize);
			_game->getEngine()->getDrawEngine()->loadDynamicText(key, { r, g, b }, fontName, fontSize);
		}*/

		void HighScoreInputState::initKeyboard(GameBase &game)
		{
		/*	_keyboard = new std::vector<Menu*>();
			char letter = 'A';
			int hpos = 50;
			int vpos = 250;
			int nums = 0;
			bool az = true;
			std::stringstream ss;
			std::string key;

			for (auto j = 0, jlen = 6; j < jlen; ++j) {
				_keyboard->push_back(new Menu(hpos, vpos, game));
				for (auto i = 0, ilen = 6; i < ilen; ++i) {
					ss.clear();
					key.clear();

					if (az) {
						ss << static_cast<char>(letter++);
						ss >> key;
					}
					else
						key = std::to_string(nums++);

					_keyboard->at(j)->addMenuTextItem(key, (std::function<void()>)[&] { 
						if (_highscoreInitials->size() < 3)
							_highscoreInitials->append(key);
					});

					if (key == "Z")
						az = false;

					_keyboard->at(j)->setWidth(keyWidth);
				}
				hpos += 76;
				break;
			}*/	

			_keyboard = new std::vector<Menu*>();
			int hpos = 50;
			int vpos = 250;
			int keyWidth = 66;
			int i = 0;


			// Keyboard column 0
			_keyboard->push_back(new Menu(hpos, vpos, game));
			_keyboard->at(i)->addMenuTextItem("Backspace", (std::function<void()>)[&] {
				if (_highscoreInitials->size() > 0)
					_highscoreInitials->pop_back();
			});
			_keyboard->at(i)->setWidth(keyWidth * 4);

			_keyboard->at(i)->addMenuTextItem("Confirm", (std::function<void()>)[&] {
				//TODO save highscore
			});
			_keyboard->at(i)->setWidth(keyWidth * 4);

			_keyboard->at(i)->addMenuTextItem("Cancel", (std::function<void()>)[&] {
				game.getStateManager()->popState();
			});
			_keyboard->at(i)->setWidth(keyWidth * 4);

			// Keyboard column 1
			i += 1;
			hpos += keyWidth * 4 + 20;
			_keyboard->push_back(new Menu(hpos, vpos, game));
			_keyboard->at(i)->addMenuTextItem("A", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("A");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("B", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("B");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("C", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("C");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("D", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("D");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("E", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("E");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("F", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("F");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			// Keyboard column 2
			i += 1;
			hpos += 76;
			_keyboard->push_back(new Menu(hpos, vpos, game));
			_keyboard->at(i)->addMenuTextItem("G", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("G");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("H", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("H");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("I", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("I");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("J", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("J");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("K", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("K");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("L", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("L");
			});

			// Keyboard column 3
			i += 1;
			hpos += 76;
			_keyboard->push_back(new Menu(hpos, vpos, game));
			_keyboard->at(i)->addMenuTextItem("M", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("M");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("N", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("N");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("O", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("O");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("P", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("P");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("Q", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("Q");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("R", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("R");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			// Keyboard column 4
			i += 1;
			hpos += 76;
			_keyboard->push_back(new Menu(hpos, vpos, game));
			_keyboard->at(i)->addMenuTextItem("S", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("S");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("T", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("T");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("U", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("U");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("V", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("V");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("W", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("W");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("X", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("X");
			});

			// Keyboard column 5
			i += 1;
			hpos += 76;
			_keyboard->push_back(new Menu(hpos, vpos, game));
			_keyboard->at(i)->addMenuTextItem("Y", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("Y");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("Z", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("Z");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("0", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("0");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("1", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("1");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("2", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("2");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("3", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("3");
			});

			// Keyboard column 6
			i += 1;
			hpos += 76;
			_keyboard->push_back(new Menu(hpos, vpos, game));
			_keyboard->at(i)->addMenuTextItem("4", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("4");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("5", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("5");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("6", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("6");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("7", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("7");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("8", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("8");
			});
			_keyboard->at(i)->setWidth(keyWidth);

			_keyboard->at(i)->addMenuTextItem("9", (std::function<void()>)[&] {
				if (_highscoreInitials->size() < 3)
					_highscoreInitials->append("9");
			});
		}	
	}
}