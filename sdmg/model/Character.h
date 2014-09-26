//
//
//  @ Project : SDMG
//  @ File Name : Character.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "engine\MovableGameObject.h"
#include <vector>

using namespace sdmg::engine;

namespace sdmg {
	namespace engine {
		class GameTime;
		class GameBase;
		enum CharacterState;
		class Weapon;
	}

	namespace model {
		class Character : public MovableGameObject {
		public:
			void update(GameTime *gameTime, GameBase *game);
		private:
			Weapon *_currentWeapon;
			std::vector<Weapon> _weapons;
			float _speed;
			Direction _direction;
			CharacterState _state;
			int _armor;
			void attack();
			void walk();
		};
	}
}