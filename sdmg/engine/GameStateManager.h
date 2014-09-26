//
//
//  @ Project : SDMG
//  @ File Name : GameStateManager.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include <stack>

namespace sdmg {
	namespace engine {
		class GameState;

		class GameStateManager {
		public:
			GameState* close();
			void open(GameState *gameState);
			void update();
		private:
			std::stack<GameState*> _currentStates;
		};
	}
}