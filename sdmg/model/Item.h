//
//
//  @ Project : SDMG
//  @ File Name : Item.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "engine\GameObject.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace engine {
		class GameTime;
		class GameBase;
	}

	namespace model {
		class Item : public GameObject {
		public:
			void update(GameTime *gameTime, GameBase *game);
		};
	}
}