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
#include <stdio.h>
#include <ctype.h>

namespace sdmg {
	namespace gamestates {
		void HighScoreInputState::init(GameBase &game)
		{
			_game = &game;
			game.getEngine()->getDrawEngine()->load("highscoreinput_background", "assets/screens/mainmenu");

			_highscoreInitials = new std::string();
			_highscore = 0;
			
			// Create menu
			_menu = new Menu(50, 250, game);
			_menu->addMenuTextItem("Clear", (std::function<void()>)[&] {
				_highscoreInitials = new std::string();
			});
			_menu->addMenuTextItem("Confirm", (std::function<void()>)[&] {
				if (!_highscoreInitials->empty()) {
					ProgressManager::getInstance().addHighscore(*_highscoreInitials, _highscore);
					ProgressManager::getInstance().save();
					game.getStateManager()->popState();
				}
				else 
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Warning", "Empty input", NULL);
			});
			_menu->addMenuTextItem("Cancel", (std::function<void()>)[&] { 
				game.getStateManager()->popState(); 
			});

			// Load text
			loadText("highscoreinput_title", "Highscore: Input your Initials", "trebucbd", 48);
			_game->getEngine()->getDrawEngine()->loadDynamicText("highscoreinput_initials", { 255, 255, 255 }, "trebucbd", 240);

			game.getEngine()->getInputEngine()->setMouseEnabled();
		}

		void HighScoreInputState::cleanup(GameBase &game)
		{
			delete _menu;
			_menu = nullptr;

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
					int keypressed = event.key.keysym.sym;
					switch (keypressed)
					{
					case SDLK_ESCAPE:
						break;
					case SDLK_BACKSPACE:
						if (!_highscoreInitials->empty())
							_highscoreInitials->pop_back();
						break;
					case SDLK_KP_ENTER:
					case SDLK_RETURN:
					case 10:
						//_keyboard.at(0)->doAction();
						/*if (_highscoreInitials->size() < 3)
							_highscoreInitials->append("X");*/
						break;
					case SDLK_0:
					case SDLK_1:
					case SDLK_2:
					case SDLK_3:
					case SDLK_4:
					case SDLK_5:
					case SDLK_6:
					case SDLK_7:
					case SDLK_8:
					case SDLK_9:
					case SDLK_a:
					case SDLK_b:
					case SDLK_c:
					case SDLK_d:
					case SDLK_e:
					case SDLK_f:
					case SDLK_g:
					case SDLK_h:
					case SDLK_i:
					case SDLK_j:
					case SDLK_k:
					case SDLK_l:
					case SDLK_m:
					case SDLK_n:
					case SDLK_o:
					case SDLK_p:
					case SDLK_q:
					case SDLK_r:
					case SDLK_s:
					case SDLK_t:
					case SDLK_u:
					case SDLK_v:
					case SDLK_w:
					case SDLK_x:
					case SDLK_y:
					case SDLK_z:
						if (_highscoreInitials->size() < 3) {
							std::stringstream ss;
							std::string key;

							ss << static_cast<char>(toupper(keypressed));
							ss >> key;
							_highscoreInitials->append(key);
						}
						break;
					default:
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
				drawEngine->drawDynamicText("highscoreinput_initials", *_highscoreInitials, 840 - drawEngine->getDynamicTextWidth("highscoreinput_initials")/2, 200);

			_menu->draw(_game);

			game.getEngine()->getDrawEngine()->render();
		}

		void HighScoreInputState::loadText(std::string key, std::string text, std::string fontName, int fontSize)
		{
			_game->getEngine()->getDrawEngine()->loadText(key, text, { 255, 255, 255 }, fontName, fontSize);
		}
	}
}