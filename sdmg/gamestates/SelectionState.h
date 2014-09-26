//
//
//  @ Project : SDMG
//  @ File Name : SelectionState.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "MenuState.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace gamestates {
		class SelectionState : public MenuState {
		public:
			void update(GameTime *gameTime);
		};
	}
}