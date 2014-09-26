//
//
//  @ Project : SDMG
//  @ File Name : GameStateManager.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "GameStateManager.h"
#include "GameState.h"

namespace sdmg {
	namespace engine {
		GameState* GameStateManager::close() {
			_currentStates.pop();
			return _currentStates.top();			
		}
		
		void GameStateManager::open(GameState *gameState) {
		
		}
		
		void GameStateManager::update() {
		
		}
		
	}
}
