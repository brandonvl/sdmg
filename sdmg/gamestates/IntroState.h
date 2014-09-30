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
			void init();
			void cleanup();

			void pause();
			void resume();

			void handleEvents(GameBase *game);
			void update(GameBase *game);
			void draw(GameBase *game);
			static IntroState* getInstance() {
				return &_instance;
			}
		protected:
			IntroState() { }
		private:
			static IntroState _instance;
		};
	}
}
