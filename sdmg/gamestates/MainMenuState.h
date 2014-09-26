//
//
//  @ Project : SDMG
//  @ File Name : MainMenuState.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "MenuState.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace engine {
		class GameTime;
	}

	namespace gamestates {
		class MainMenuState : public MenuState {
		public:
			void update(GameTime *gameTime);
		};
	}
}