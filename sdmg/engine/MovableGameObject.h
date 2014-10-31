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
			enum class State {
				WALKING, RUNNING, IDLE, FORWARD_ROLL, KNEELING, RESPAWN,
				JUMPING, JUMPINGLEFT, JUMPINGRIGHT,
				FALLING, FALLINGLEFT, FALLINGRIGHT,
				KNOCKBACKLEFT, KNOCKBACKRIGHT,
				SHORTRANGEATTACK,
				MIDRANGEATTACKBEGIN, MIDRANGEATTACK, MIDRANGEATTACKEND,
				LONGRANGEATTACK };
			enum class Direction { LEFT, UP, DOWN, RIGHT };

			MovableGameObject();
			virtual ~MovableGameObject();
			void setSpeed(Speed speed);
			void setSpeed(float horizontalSpeed, float verticalSpeed);
			void update(GameTime *gameTime, GameBase *game);
			float getHorizontalSpeed();
			float getVerticalSpeed();
			Speed getSpeed();

			bool stateIsInterruptible();

			State getState();
			void setState(State state);
			Direction getDirection();
			void setDirection(Direction direction);
			void stateCompleted();
			Direction getSpawnDirection();
			void setSpawnDirection(Direction direction);

			int getLives();
			void setLives(int lives);
			int getHP();
			void setHP(int hp);
			bool getIsJumping();
			void setIsJumping(bool isJumping);
			
			
			
			
			b2Body* getAttackBody();
			void setAttackBody(b2Body *attackBody);
			float getAttackY();
			void setAttackY(float y);
			float MovableGameObject::getAttackWidth();
			float MovableGameObject::getAttackHeight();
			void MovableGameObject::setAttackSize(Size size);
			void MovableGameObject::setAttackSize(float width, float height);

		private:
			Direction _spawnDirection;
			int _lives, _hp;
			Size _attackSize;
			float _attackY;
		protected:
			b2Body *_attackBody;
			Direction _direction;
			bool _isJumping;
			Speed _speed;
			State _state;
		};
	}
}