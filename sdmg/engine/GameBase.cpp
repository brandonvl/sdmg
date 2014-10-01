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

namespace sdmg {
	namespace engine {
		GameBase::GameBase() {
			internalInitialize();
		}

		void GameBase::start() {
			_running = true;
			doGameLoop();		
		}

		void GameBase::internalInitialize() {
			_gameTime = new GameTime();
			_world = new World();
			_gameStateManager = new GameStateManager(this);
			_engine = new Engine();
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
			SDL_Event event;
			while (_running) {
				internalDraw();
				if (SDL_PollEvent(&event))
				{
					if (event.type == SDL_QUIT)
					{
						_running = false;
					}

					if (event.type == SDL_KEYDOWN)
					{
						switch (event.key.keysym.sym)
						{
						case SDLK_ESCAPE:
							_running = false;
							break;
						}
					}
				}
			}
		}
		
	}
}
