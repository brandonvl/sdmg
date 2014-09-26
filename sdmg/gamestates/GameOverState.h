//
//
//  @ Project : SDMG
//  @ File Name : GameOverState.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "engine\GameState.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace gamestates {
		class GameOverState : public GameState {
		public:
			void update(GameTime *gameTime);
		};
	}
}