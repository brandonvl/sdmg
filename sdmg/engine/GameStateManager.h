//
//
//  @ Project : SDMG
//  @ File Name : GameStateManager.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include <vector>

namespace sdmg {
	namespace engine {
		class GameState;
		class GameBase;

		class GameStateManager {
		public:
			GameStateManager(GameBase *engine);
			virtual ~GameStateManager();
			void cleanup();
			void changeState(GameState &state);
			void pushState(GameState &state);
			void popState();

			void handleEvents();
			void update();
			void draw();
		private:
			std::vector<GameState*> _states;
			GameBase *_engine;
		};
	}
}