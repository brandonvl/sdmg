//
//
//  @ Project : SDMG
//  @ File Name : AICharacter.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "Character.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace engine {
		class GameTime;
		class GameBase;
	}

	namespace model {
		class AICharacter : public Character {
		public:
			void update(GameTime *gameTime, GameBase *game);
		};
	}
}