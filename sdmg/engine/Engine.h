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
		namespace drawing {
			class DrawEngine;
		}
		using namespace sdmg::engine::drawing;

		class PhysicsEngine;
		class AudioEngine;
		class InputEngine;
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
		private:
			DrawEngine *_drawEngine;
			PhysicsEngine *_physicsEngine;
			AudioEngine *_audioEngine;
			InputEngine *_inputEngine;
			GameBase *_game;
		};
	}
}