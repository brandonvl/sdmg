//
//
//  @ Project : SDMG
//  @ File Name : Engine.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "Engine.h"
#include "GameState.h"
#include "GameTime.h"
#include "GameBase.h"

namespace sdmg {
	namespace engine {
		void Engine::update(const GameState *gameState, const GameTime *gameTime) {
		
		}
		
		GameBase* Engine::getGame() {
			return _game;
		}
		
		PhysicsEngine* Engine::getPhysicsEngine() {
			return _physicsEngine;
		}
		
		DrawEngine* Engine::getDrawEngine() {
			return _drawEngine;
		}
		
		AudioEngine* Engine::getAudioEngine() {
			return _audioEngine;
		}
		
		InputEngine* Engine::getInputEngine() {
			return _inputEngine;
		}
		
	}
}
