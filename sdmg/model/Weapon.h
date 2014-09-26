//
//
//  @ Project : SDMG
//  @ File Name : Weapon.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "engine\GameObject.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace model {
		class Character;

		class Weapon : public GameObject {
		public:
			void update(GameTime *gameTime, GameBase *game);
		private:
			int _cooldown;
			int _damage;
			Character *_character;
		};
	}
}