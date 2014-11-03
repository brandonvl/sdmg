//
//
//  @ Project : SDMG
//  @ File Name : TutorialState.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#include "TutorialState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "LoadingState.h"
#include "engine\physics\PhysicsEngine.h"
#include "model\Platform.h"
#include "model\MovablePlatform.h"
#include "model\Character.h"
#include "factories\CharacterFactory.h"
#include "engine\input\InputEngine.h"
#include "actions\Actions.h"
#include "MainMenuState.h"
#include "GameOverState.h"
#include "engine\World.h"
#include "engine\audio\AudioEngine.h"
#include "helperclasses\ConfigManager.h"

namespace sdmg {
	namespace gamestates {
		void TutorialState::init(GameBase &game)
		{
			PlayState::init(game);

			_tutorial = new std::vector<std::pair<SDL_Keycode, std::string>>();
			_toDraw = new std::vector<std::string>();
			
			ConfigManager &manager = ConfigManager::getInstance();

			_tutorial->push_back(std::make_pair(manager.getKey(0, "roll"), "tutEnd"));
			// Panda
			_tutorial->push_back(std::make_pair(manager.getKey(0, "midrange"), "tutFiat7"));
			_tutorial->push_back(std::make_pair(SDLK_RETURN, "tutFiat6"));
			_tutorial->push_back(std::make_pair(manager.getKey(0, "jump"), "tutFiat5"));
			_tutorial->push_back(std::make_pair(manager.getKey(0, "walkRight"), "tutFiat4"));
			_tutorial->push_back(std::make_pair(manager.getKey(0, "walkLeft"), "tutFiat3"));
			_tutorial->push_back(std::make_pair(SDLK_RETURN, "tutFiat2"));
			_tutorial->push_back(std::make_pair(manager.getKey(1, "roll"), "tutFiat1"));
			// Nivek

			_tutorial->push_back(std::make_pair(manager.getKey(1, "midrange"), "tutNivek6"));
			_tutorial->push_back(std::make_pair(SDLK_RETURN, "tutNivek5"));
			_tutorial->push_back(std::make_pair(manager.getKey(1, "jump"), "tutNivek4"));
			_tutorial->push_back(std::make_pair(manager.getKey(1, "walkRight"), "tutNivek3"));
			_tutorial->push_back(std::make_pair(manager.getKey(1, "walkLeft"), "tutNivek2"));
			_tutorial->push_back(std::make_pair(SDLK_RETURN, "tutNivek1"));
			_tutorial->push_back(std::make_pair(SDLK_RETURN, "tutIntro4"));
			_tutorial->push_back(std::make_pair(SDLK_RETURN, "tutIntro3"));
			_tutorial->push_back(std::make_pair(SDLK_RETURN, "tutIntro2"));

			_toDraw->push_back("tutIntro");
		}
		
		void TutorialState::cleanup(GameBase &game)
		{
			PlayState::cleanup(game);

			game.getEngine()->getDrawEngine()->unloadText("tutIntro");
			game.getEngine()->getDrawEngine()->unloadText("tutIntro2");
			game.getEngine()->getDrawEngine()->unloadText("tutIntro3");
			game.getEngine()->getDrawEngine()->unloadText("tutIntro4");
			game.getEngine()->getDrawEngine()->unloadText("tutNivek1");
			game.getEngine()->getDrawEngine()->unloadText("tutNivek2");
			game.getEngine()->getDrawEngine()->unloadText("tutNivek3");
			game.getEngine()->getDrawEngine()->unloadText("tutNivek4");
			game.getEngine()->getDrawEngine()->unloadText("tutNivek5");
			game.getEngine()->getDrawEngine()->unloadText("tutNivek6");
			game.getEngine()->getDrawEngine()->unloadText("tutFiat1");
			game.getEngine()->getDrawEngine()->unloadText("tutFiat2");
			game.getEngine()->getDrawEngine()->unloadText("tutFiat3");
			game.getEngine()->getDrawEngine()->unloadText("tutFiat4");
			game.getEngine()->getDrawEngine()->unloadText("tutFiat5");
			game.getEngine()->getDrawEngine()->unloadText("tutFiat6");
			game.getEngine()->getDrawEngine()->unloadText("tutFiat7");
			game.getEngine()->getDrawEngine()->unloadText("tutEnd");

			delete _tutorial;
			delete _toDraw;
		}

		void TutorialState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				game.getEngine()->getInputEngine()->handleControllers(event);
				switch (event.type) {
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						changeState(game, MainMenuState::getInstance());
						break;
					default:
						if (event.type == SDL_KEYDOWN && _tutorial->size() > 0) {
							SDL_Keycode key = _tutorial->back().first;
							if (key == event.key.keysym.sym)
							{
								_toDraw->push_back(_tutorial->back().second);
								_tutorial->pop_back();
							}
						}

						game.getEngine()->getInputEngine()->handleEvent(event);
						break;
					}
					break;
				case SDL_QUIT:
					game.stop();
					break;
				}
			}
		}

		void TutorialState::update(GameBase &game, GameTime &gameTime)
		{
			PlayState::update(game, gameTime);

			

		}

		void TutorialState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();

			preformDraw(game);
			
			if (_toDraw->size() > 0) {
				std::string key = _toDraw->back();
				std::array<float, 2> size = game.getEngine()->getDrawEngine()->getTextSize(key);
				float f = size.at(0); // Text width
				int width = game.getEngine()->getDrawEngine()->getWindowWidth(); // Window width
				int x = (width / 2) - (f / 2); // Draw text at center
				game.getEngine()->getDrawEngine()->drawText(key, x, 200);
			}

			game.getEngine()->getDrawEngine()->render();
		}
	}
}
