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
#include "helperclasses\HUD.h"
#include "engine\ai\TutorialMachine.h"
#include "engine\ai\EasyAIMachine.h"

namespace sdmg {
	namespace gamestates {
		void TutorialState::init(GameBase &game)
		{
			PlayState::init(game);

			_step = 1.0f / 4.0f;
			_lastUpdate = std::chrono::high_resolution_clock::now();
			_canDie = false;

			
			Character *character = static_cast<Character*>(game.getWorld()->getPlayers()[0]);
			Character *enemy = static_cast<Character*>(game.getWorld()->getPlayers()[1]);
			// 25.0F,1.0F
			engine::ai::AIMachine *machine = new engine::ai::TutorialMachine(*character, *enemy,25.0F,1.0F);

			character->setAI(*machine);

			_tutorial = new std::vector<std::pair<SDL_Keycode, std::string>>();
			_toDraw = new std::vector<std::string>();
			
			ConfigManager &manager = ConfigManager::getInstance();

			_tutorial->push_back(std::make_pair(manager.getKey(0, "roll"), "tutEnd"));
			// Panda
			//_tutorial->push_back(std::make_pair(manager.getKey(0, "midrange"), "tutFiat7"));
			//_tutorial->push_back(std::make_pair(SDLK_RETURN, "tutFiat6"));
			//_tutorial->push_back(std::make_pair(manager.getKey(0, "jump"), "tutFiat5"));
			//_tutorial->push_back(std::make_pair(manager.getKey(0, "walkRight"), "tutFiat4"));
			//_tutorial->push_back(std::make_pair(manager.getKey(0, "walkLeft"), "tutFiat3"));
			//_tutorial->push_back(std::make_pair(SDLK_RETURN, "tutFiat2"));
			//_tutorial->push_back(std::make_pair(manager.getKey(1, "roll"), "tutFiat1"));
			
			// Nivek
			_tutorial->push_back(std::make_pair(manager.getKey(2, "roll"), "tutEnd"));
			_tutorial->push_back(std::make_pair(manager.getKey(2, "block"), "tutNivek8"));
			_tutorial->push_back(std::make_pair(manager.getKey(2, "longrange"), "tutNivek7"));
			_tutorial->push_back(std::make_pair(manager.getKey(2, "midrange"), "tutNivek6"));
			_tutorial->push_back(std::make_pair(SDLK_RETURN, "tutNivek5"));
			_tutorial->push_back(std::make_pair(manager.getKey(2, "jump"), "tutNivek4"));
			_tutorial->push_back(std::make_pair(manager.getKey(2, "walkRight"), "tutNivek3"));
			_tutorial->push_back(std::make_pair(manager.getKey(2, "walkLeft"), "tutNivek2"));
			_tutorial->push_back(std::make_pair(SDLK_RETURN, "tutNivek1"));
			_tutorial->push_back(std::make_pair(SDLK_RETURN, "tutIntro4"));
			_tutorial->push_back(std::make_pair(SDLK_RETURN, "tutIntro3"));
			_tutorial->push_back(std::make_pair(SDLK_RETURN, "tutIntro2"));

			_toDraw->push_back("tutIntro");
		}
		
		void TutorialState::cleanup(GameBase &game)
		{
			PlayState::cleanup(game);

			if (_huds) {
				for (auto it : *_huds) {
					delete it;
				}
				_huds->clear();
			}

			delete _level;
			_level = nullptr;
			delete _huds;
			_huds = nullptr;

			/*
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
			*/

			game.getWorld()->destroyShootBodies();
			game.getEngine()->getPhysicsEngine()->cleanUp();
			game.getEngine()->getDrawEngine()->unloadAll();
			game.getEngine()->getAudioEngine()->unloadAll();
			game.getEngine()->getInputEngine()->clearBindings();
			game.getWorld()->clearWorld();
			

			delete _tutorial;
			delete _toDraw;

			delete _editor;
			_editor = nullptr;
		}

		void TutorialState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				//game.getEngine()->getInputEngine()->handleControllers(event);
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

			 /*
			for (auto it : game.getWorld()->getPlayers()) {
				it->update(&gameTime, &game);
			}
			
			auto curTime = std::chrono::high_resolution_clock::now();
			float diff = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - _lastUpdate).count() / 1000.0f;

			_lastUpdate = curTime;
			_accumulator += diff;

			while (_accumulator > _step) {
				for (auto obj : game.getWorld()->getPlayers())
				{
					obj->addPP(1);
				}
				_accumulator -= _step;
			}

			if (_showFPS)
				_fps = game.getFPS() == _fps ? _fps : game.getFPS();

			game.getEngine()->getInputEngine()->update(game);
			game.getEngine()->getDrawEngine()->update();
			game.getEngine()->getPhysicsEngine()->update();
			*/
		}

		void TutorialState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();

			performDraw(game);
			
			if (_toDraw->size() > 0) {
				std::string key = _toDraw->back();
				std::array<int, 2> size = game.getEngine()->getDrawEngine()->getTextSize(key);
				float f = size.at(0); // Text width
				int width = game.getEngine()->getDrawEngine()->getWindowWidth(); // Window width
				int x = (width / 2) - (f / 2); // Draw text at center
				game.getEngine()->getDrawEngine()->drawText(key, x, 200);
			}

			game.getEngine()->getDrawEngine()->render();
		}
	}
}
