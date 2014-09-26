//
//
//  @ Project : SDMG
//  @ File Name : LevelSelectionState.h
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
		class LevelSelectionState : public SelectionState {
		public:
			void update(GameTime *gameTime);
		};
	}
}