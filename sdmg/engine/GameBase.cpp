//
//
//  @ Project : SDMG
//  @ File Name : GameBase.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "GameBase.h"
#include "World.h"
#include "Engine.h"
#include "GameStateManager.h"
#include "GameTime.h"
#include "GameState.h"
#include "sdl\include\SDL.h"
#include "drawing\DrawEngine.h"
#include "gamestates\IntroState.h"
#include "gamestates\MainMenuState.h"
#include "gamestates\PlayState.h"
#include "helperclasses\ConfigManager.h"
#include "helperclasses\ProgressManager.h"

namespace sdmg {
	namespace engine {
		GameBase::GameBase() {
			internalInitialize();
		}

		GameBase::~GameBase() {
			helperclasses::ConfigManager::getInstance().cleanup();
			helperclasses::ProgressManager::getInstance().cleanup();
			delete _gameTime;
			delete _world;
			delete _engine;
			delete _gameStateManager;
		}

		void GameBase::start() {
			_running = true;
			_gameStateManager->changeState(gamestates::MainMenuState::getInstance());
			doGameLoop();		
		}

		void GameBase::internalInitialize() {
			_gameTime = new GameTime();
			_world = new World();
			_engine = new Engine();
			_gameStateManager = new GameStateManager(this);
		}
		
		void GameBase::stop() {
			_running = false;
		}
		
		World* GameBase::getWorld() {
			return _world;
		}

		long GameBase::getFPS() {
			//return _gameTime->getElapsedSinceLastUpdate() / 1.0;
			return _avgFPS;
		}
		
		void GameBase::internalDraw() {

		}
		
		Engine* GameBase::getEngine() {
			return _engine;
		}
		
		void GameBase::internalUpdate() {
		
		}

		void GameBase::doGameLoop() {


			Uint32 fps_lasttime = SDL_GetTicks(); //the last recorded time.
			Uint32 fps_current; //the current FPS.
			Uint32 fps_frames = 0; //frames passed since the last recorded fps
			//SDL_Event event;
			while (_running) {
				_gameStateManager->handleEvents();
				_gameStateManager->update();
				_gameStateManager->draw();

				_gameTime->update();

				fps_frames++;
				if (fps_lasttime < SDL_GetTicks() - 1000.0F)
				{
					fps_lasttime = SDL_GetTicks();
					_avgFPS = fps_frames;
					fps_frames = 0;
				}
			}
			_gameStateManager->cleanup();
		}
	}
}
