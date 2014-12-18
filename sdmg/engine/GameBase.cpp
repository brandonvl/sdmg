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
#include "helperclasses\RandomGenerator.h"
#include "helperclasses\Recorder.h"

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
			delete _randomGenerator;
			delete _recorder;
		}

		void GameBase::start() {
			_running = true;
			_gameStateManager->changeState(gamestates::MainMenuState::getInstance());
			doGameLoop();
		}

		void GameBase::internalInitialize() {
			_gameTime = new GameTime();
			_world = new World();
			_engine = new Engine(*this);
			_gameStateManager = new GameStateManager(this);
			_randomGenerator = new helperclasses::RandomGenerator();
			_recorder = new helperclasses::Recorder();
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

		GameTime *GameBase::getGameTime()
		{
			return _gameTime;
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

		GameBase::GameMode GameBase::getGameMode()
		{
			return _gameMode;
		}

		void GameBase::setGameMode(GameBase::GameMode mode)
		{
			_gameMode = mode;
		}
	}
}
