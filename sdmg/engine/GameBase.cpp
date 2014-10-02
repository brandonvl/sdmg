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

namespace sdmg {
	namespace engine {
		GameBase::GameBase() {
			internalInitialize();
		}

		void GameBase::start() {
			_running = true;
			_gameStateManager->changeState(gamestates::IntroState::getInstance());
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
		
		void GameBase::internalDraw() {
			_engine->getDrawEngine()->draw("chicken", Rectangle(0, 0, 100, 100));
		}
		
		Engine* GameBase::getEngine() {
			return _engine;
		}
		
		void GameBase::internalUpdate() {
		
		}

		void GameBase::doGameLoop() {
			//SDL_Event event;
			while (_running) {
				_gameStateManager->handleEvents();
				_gameStateManager->update();
				_gameStateManager->draw();

				_gameTime->update();
			}
			_gameStateManager->cleanup();
		}
	}
}
