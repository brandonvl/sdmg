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

namespace sdmg {
	namespace model {
		Character::Character()
		{
			
		}

		Character::~Character() { }

		std::string Character::getName()
		{
			return _name;
		}

		void Character::setName(std::string name)
		{
			_name = name;
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
		
		}
		
		void Character::attack() {
		
		}
		
		void Character::walk() {
		
		}
		
	}
}
