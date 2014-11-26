//
//
//  @ Project : SDMG
//  @ File Name : Engine.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once



namespace sdmg {
	namespace engine {
		
		namespace drawing { class DrawEngine; }
		namespace physics { class PhysicsEngine; }
		namespace audio { class AudioEngine; }
		namespace input { class InputEngine; }
		namespace particle { class ParticleEngine; }

		using namespace sdmg::engine::drawing;
		using namespace sdmg::engine::physics;
		using namespace sdmg::engine::audio;
		using namespace sdmg::engine::input;
		using namespace sdmg::engine::particle;
		
		class GameState;
		class GameTime;
		class GameBase;

		class Engine {
		public:
			Engine();
			virtual ~Engine();
			void update(const GameState *gameState, const GameTime *gameTime);
			GameBase* getGame();
			PhysicsEngine* getPhysicsEngine();
			DrawEngine* getDrawEngine();
			AudioEngine* getAudioEngine();
			InputEngine* getInputEngine();
			ParticleEngine* getParticleEngine();
		private:
			DrawEngine *_drawEngine;
			PhysicsEngine *_physicsEngine;
			AudioEngine *_audioEngine;
			InputEngine *_inputEngine;
			ParticleEngine *_particleEngine;
			GameBase *_game;
		};
	}
}