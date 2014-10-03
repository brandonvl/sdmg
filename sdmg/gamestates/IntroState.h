//
//
//  @ Project : SDMG
//  @ File Name : IntroState.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once

#include "engine\GameState.h"

using namespace sdmg::engine;



namespace sdmg {
	namespace gamestates {
		
		class IntroState : public GameState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void pause(GameBase &game);
			void resume(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);
			static IntroState& getInstance() {
				static IntroState _instance;
				return _instance;
			}
		protected:
			IntroState() { }
		};
	}
}
