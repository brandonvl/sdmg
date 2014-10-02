//
//
//  @ Project : SDMG
//  @ File Name : MovableGameObject.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "GameObject.h"

namespace sdmg {
	namespace engine {
		class GameTime;
		class GameBase;
		enum class Direction;
		struct Speed
		{
			Speed::Speed() { }
			Speed::Speed(float horizontalSpeed, float verticalSpeed)
			{
				horizontal = horizontalSpeed;
				vertical = verticalSpeed;
			}
			float horizontal, vertical;
		};

		class MovableGameObject : public GameObject {
		public:
			MovableGameObject();
			virtual ~MovableGameObject();
			void setSpeed(Speed speed);
			void setSpeed(float horizontalSpeed, float verticalSpeed);
			void update(GameTime *gameTime, GameBase *game);
			float getHorizontalSpeed();
			float getVerticalSpeed();
			Speed getSpeed();
		private:
			Direction _direction;
			Speed _speed;
		};
	}
}