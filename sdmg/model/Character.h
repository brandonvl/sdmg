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
		namespace ai {
			class AIMachine;
		}
	}

	namespace model {
		class Character : public MovableGameObject {
		public:
			Character();
			virtual ~Character();
			void update(GameTime *gameTime, GameBase *game);
			std::string getKey();
			void setKey(std::string key);
			void die() override;
			void revive();
			void setAI(ai::AIMachine &machine) { _aiMachine = &machine; }
			ai::AIMachine *getAI() { return _aiMachine; }
			bool isAI()override { return _aiMachine; }
		private:
			Weapon *_currentWeapon;
			std::vector<Weapon*> _weapons;
			std::string _key;
			float _speed;
			Direction _direction;
			CharacterState _state;
			int _armor;
			ai::AIMachine *_aiMachine;
		};
	}
}