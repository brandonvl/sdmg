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

namespace sdmg {
	namespace engine {
		void GameBase::start() {
			_running = true;
			doGameLoop();			
		}
		
		void GameBase::stop() {
			_running = false;
		}
		
		World* GameBase::getWorld() {
			return _world;
		}
		
		void GameBase::internalDraw() {
			
		}
		
		void GameBase::internalInitialize() {
		
		}
		
		Engine* GameBase::getEngine() {
			return _engine;
		}
		
		void GameBase::internalUpdate() {
		
		}

		void GameBase::doGameLoop() {
			while (_running) {
				internalUpdate();
				internalDraw();
			}
		}
		
	}
}
