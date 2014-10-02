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
		MovableGameObject::MovableGameObject()
		{

		}

		MovableGameObject::~MovableGameObject() { }

		void MovableGameObject::update(GameTime *gameTime, GameBase *game) {
			_speed = Speed(0.0f, 0.0f);
		}

		Speed MovableGameObject::getSpeed()
		{
			return _speed;
		}


		float MovableGameObject::getHorizontalSpeed()
		{
			return _speed.horizontal;
		}

		float MovableGameObject::getVerticalSpeed()
		{
			return _speed.vertical;
		}

		void MovableGameObject::setSpeed(Speed speed)
		{
			_speed = speed;
		}

		void MovableGameObject::setSpeed(float horizontalSpeed, float verticalSpeed)
		{
			_speed = Speed(horizontalSpeed, verticalSpeed);
		}
	}
}
