//
//
//  @ Project : SDMG
//  @ File Name : CharacterSelectionState.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "SelectionState.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace engine {
		class GameTime;
	}

	namespace gamestates {
		class CharacterSelectionState : public SelectionState {
		public:
			void update(GameTime *gameTime);
		};
	}
}