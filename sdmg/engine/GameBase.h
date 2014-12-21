//
//
//  @ Project : SDMG
//  @ File Name : GameBase.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once

namespace sdmg {

	namespace helperclasses {
		class RandomGenerator;
		class Recorder;
	}

	namespace engine {
		class World;
		class Engine;
		class GameStateManager;
		class GameTime;
		class GameState;

		class GameBase {
		public:
			GameBase();
			virtual ~GameBase();
			void start();
			void stop();
			World* getWorld();
			void internalDraw();
			Engine* getEngine();
			long getFPS();
			GameTime *getGameTime();
			GameStateManager *getStateManager() { return _gameStateManager; };
			helperclasses::RandomGenerator *getRandomGenerator() { return _randomGenerator; }
			helperclasses::Recorder &getRecorder() { return *_recorder; }

			enum GameMode
			{
				SinglePlayer,
				Versus,
				Survival,
				Playback
			};

			GameMode getGameMode();
			void setGameMode(GameMode mode);

		protected:
			virtual void update(GameState *gameState, GameTime *gameTime) = 0;
			virtual void draw(GameState *gameState, GameTime *gameTime) = 0;
			virtual void initialize() = 0;
		private:
			GameStateManager *_gameStateManager;
			helperclasses::RandomGenerator *_randomGenerator;
			GameTime *_gameTime;
			Engine *_engine;
			helperclasses::Recorder *_recorder;
			bool _running;
			bool _calculateFPS;
			GameMode _gameMode;
			float _avgFPS;
			World *_world;
			void internalUpdate();
			void doGameLoop();
			void internalInitialize();

			friend class GameStateManager;
			
		};
	}
}