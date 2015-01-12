//
//
//  @ Project : SDMG
//  @ File Name : Character.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "Character.h"
#include "engine\GameTime.h"
#include "engine\GameBase.h"
#include "engine\World.h"
#include "engine\ai\AIMachine.h"

namespace sdmg {
	namespace model {
		Character::Character()
		{
			_flags = Flags::CANDIE;
			setState(MovableGameObject::State::FALLING);
		}

		Character::~Character() {

			if (_aiMachine)
				delete _aiMachine;

		}

		std::string Character::getKey()
		{
			return _key;
		}

		void Character::setKey(std::string key)
		{
			_key = key;
		}

		void Character::update(GameTime *gameTime, GameBase *game) {
			if (_aiMachine != nullptr) {
				_aiMachine->update(gameTime, game);
			}
		}
				
		void Character::die() {
			getWorld()->onDead(this);
		}

		void Character::revive()
		{
			_hp = -10;
			setState(MovableGameObject::State::RESPAWN);
			_hp = 100;
			_lives = 5;
			_body->SetActive(true);
		}
	}
}
