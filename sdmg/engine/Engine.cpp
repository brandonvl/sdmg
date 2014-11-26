//
//
//  @ Project : SDMG
//  @ File Name : Engine.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "Engine.h"
#include "drawing\DrawEngine.h"
#include "physics\PhysicsEngine.h"
#include "input\InputEngine.h"
#include "audio\AudioEngine.h"
#include "particle\ParticleEngine.h"
#include "GameState.h"
#include "GameTime.h"
#include "GameBase.h"

namespace sdmg {
	namespace engine {
		Engine::Engine() {
			_drawEngine = new DrawEngine();
			_physicsEngine = new PhysicsEngine(this);
			_inputEngine = new InputEngine();
			_audioEngine = new AudioEngine();
			_particleEngine = new ParticleEngine();
		}

		Engine::~Engine() {
			delete _drawEngine;
			delete _physicsEngine;
			delete _inputEngine;
			delete _audioEngine;
			delete _particleEngine;
		}

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
		
		ParticleEngine* Engine::getParticleEngine() {
			return _particleEngine;
		}
	}
}
