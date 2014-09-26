//
//
//  @ Project : SDMG
//  @ File Name : GameState.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once

namespace sdmg {
	namespace engine {
		class GameTime;
		class GameBase;

		class GameState {
		public:
			virtual void update(GameTime *gameTime) = 0;
		protected:
			GameBase *_game;
		};
	}
}