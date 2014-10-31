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

namespace sdmg {
	namespace gamestates {
		void TutorialState::init(GameBase &game)
		{
			game.getEngine()->getPhysicsEngine()->resume();
			game.getEngine()->getAudioEngine()->play("level1_bgm", 0);

			_tutorial = new std::vector<std::pair<SDL_Keycode, std::string>>();
			_toDraw = new std::vector<std::string>();

			_tutorial->push_back(std::make_pair(SDLK_KP_0, "tut7"));
			_tutorial->push_back(std::make_pair(SDLK_l, "tut6"));
			_tutorial->push_back(std::make_pair(SDLK_RETURN, "tut5"));
			_tutorial->push_back(std::make_pair(SDLK_UP, "tut4"));
			_tutorial->push_back(std::make_pair(SDLK_RIGHT, "tut3"));
			_tutorial->push_back(std::make_pair(SDLK_LEFT, "tut2"));
			_tutorial->push_back(std::make_pair(SDLK_RETURN, "tut1"));

			_toDraw->push_back("tutIntro");
		}

		void TutorialState::setCharacters(std::vector<model::Character*> *characters)
		{
			_characters = characters;
		}

		void TutorialState::setPlatform(model::Platform *platform)
		{
			_platform = platform;
		}

		void TutorialState::cleanup(GameBase &game)
		{
			game.getWorld()->clearWorld();
			game.getEngine()->getPhysicsEngine()->cleanUp();
			game.getEngine()->getDrawEngine()->unloadAll();
			game.getEngine()->getAudioEngine()->unloadAll();
			

			game.getEngine()->getDrawEngine()->unloadText("tutIntro");
			game.getEngine()->getDrawEngine()->unloadText("tut1");
			game.getEngine()->getDrawEngine()->unloadText("tut2");
			game.getEngine()->getDrawEngine()->unloadText("tut3");
			game.getEngine()->getDrawEngine()->unloadText("tut4");
			game.getEngine()->getDrawEngine()->unloadText("tut5");
			game.getEngine()->getDrawEngine()->unloadText("tut6");
			game.getEngine()->getDrawEngine()->unloadText("tut7");

			delete _tutorial;
			delete _toDraw;
		}

		void TutorialState::pause(GameBase &game)
		{
		}

		void TutorialState::resume(GameBase &game)
		{
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
						_pressed = event.key.keysym.sym;
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
			if (_pressed && _tutorial->size() > 0) {
				SDL_Keycode key = _tutorial->back().first;
				if (key == _pressed) 
				{ 
					_toDraw->push_back(_tutorial->back().second);
					_tutorial->pop_back(); 
				}

			}

			if (game.getWorld()->isGameOver()) {
				if (game.getWorld()->getAliveList().size() > 0)
					game.getWorld()->getAliveList()[0]->die();
				game.getEngine()->getPhysicsEngine()->pause();
				changeState(game, GameOverState::getInstance());
			}

			game.getEngine()->getInputEngine()->runActions(game);
			game.getEngine()->getDrawEngine()->update();
			game.getEngine()->getPhysicsEngine()->update();

			_pressed = SDL_Keycode(NULL);
		}

		void TutorialState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();

			game.getEngine()->getDrawEngine()->draw("background");

			game.getEngine()->getDrawEngine()->draw(_platform);
			game.getEngine()->getDrawEngine()->drawText("escape_text", 10, 10);

			for (int i = 0; i < _characters->size(); i++)
				game.getEngine()->getDrawEngine()->drawSlice((*_characters)[i]);

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
