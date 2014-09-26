//
//
//  @ Project : SDMG
//  @ File Name : Game.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#if !defined(_GAME_H)
#define _GAME_H

#include "engine\GameBase.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace engine {
		class GameState;
		class GameTime;
	}

	class Game : public GameBase {
	protected:
		void update(GameState *gameState, GameTime *gameTime);
		void draw(GameState *gameState, GameTime *gameTime);
		void initialize();
	};
}

#endif  //_GAME_H
