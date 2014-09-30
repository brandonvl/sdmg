//
//
//  @ Project : SDMG
//  @ File Name : MovableGameObject.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "MovableGameObject.h"
#include "GameTime.h"
#include "GameBase.h"

namespace sdmg {
	namespace engine {
		void MovableGameObject::update(GameTime *gameTime, GameBase *game) {
		
		}

		Speed MovableGameObject::getSpeed()
		{
			return _speed;
		}

		void MovableGameObject::setSpeed(Speed speed)
		{
			_speed = speed;
		}
		
	}
}
