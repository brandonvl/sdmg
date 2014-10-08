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
			enum class State { WALKING, RUNNING, JUMPING, IDLE, FORWARD_ROLL, RESPAWN };
			enum class Direction { LEFT, UP, DOWN, RIGHT };

			MovableGameObject();
			virtual ~MovableGameObject();
			void setSpeed(Speed speed);
			void setSpeed(float horizontalSpeed, float verticalSpeed);
			void update(GameTime *gameTime, GameBase *game);
			float getHorizontalSpeed();
			float getVerticalSpeed();
			Speed getSpeed();

			State getState();
			void setState(State state);
			Direction getDirection();
			void setDirection(Direction direction);
			void stateCompleted();

		protected:
			Direction _direction;
			Speed _speed;
			State _state;
		};
	}
}