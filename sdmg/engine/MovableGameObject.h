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
#include <functional>
#include <vector>

#define MoveObjState sdmg::engine::MovableGameObject::State
#define MoveObjDirection sdmg::engine::MovableGameObject::Direction

namespace sdmg {
	namespace engine {
		class GameTime;
		class GameBase;
		class Engine;
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
				WALKING = 1, RUNNING = 2, IDLE = 4, FORWARD_ROLL = 8, KNEELING = 16, RESPAWN = 32,
				JUMPING = 64, JUMPINGLEFT = 128, JUMPINGRIGHT = 256,
				FALLING = 512, FALLINGLEFT = 1024, FALLINGRIGHT = 2048,
				KNOCKBACKLEFT = 4096, KNOCKBACKRIGHT = 8192,
				BLOCKING = 16384,
				MIDRANGEATTACKBEGIN = 32768, MIDRANGEATTACK = 65536, MIDRANGEATTACKEND = 131072,
				LONGRANGEATTACKBEGIN = 262144, LONGRANGEATTACK = 524288, LONGRANGEATTACKEND = 1048576
			};
			enum class Direction { LEFT, UP, DOWN, RIGHT };
			

			MovableGameObject();
			virtual ~MovableGameObject();
			void setSpeed(Speed speed);
			void setSpeed(float horizontalSpeed, float verticalSpeed);
			virtual void update(GameTime *gameTime, GameBase *game);
			float getHorizontalSpeed();
			float getVerticalSpeed();
			Speed getSpeed();

			bool stateIsInterruptible();

			State getState();
			void setState(State state);
			bool getRolling();
			bool getFalling();
			Direction getDirection();
			void setDirection(Direction direction);
			void stateCompleted();
			Direction getSpawnDirection();
			void setSpawnDirection(Direction direction);

			int getLives();
			void setLives(int lives);

			int getHP();
			void hit(int damage);
			void setHP(int hp);

			int getMidCost();
			void setMidCost(int cost);
			int getLongCost();
			void setLongCost(int cost);

			int getMidDamage();
			void setMidDamage(int damage);
			int getLongDamage();
			void setLongDamage(int damage);

			int getPP();
			void addPP(int pp);
			void setPP(int pp);

			bool getIsJumping();
			void setIsJumping(bool isJumping);

			bool getShouldTurnArround();
			void setShouldTurnArround(bool arround);
						
			b2Body* getAttackBody();
			void setAttackBody(b2Body *attackBody);
			void destroyAttackBody();
			b2Body* getShootBody();
			void setShootBody(b2Body *shootBody);
			void destroyShootBody(Engine &engine);

			double getMultiplierOnHit();
			void setMultiplierOnHit(double multiplier);

			float getMidAttackY();
			void setMidAttackY(float y);
			float getLongAttackY();
			void setLongAttackY(float y);

			float MovableGameObject::getAttackWidth();
			float MovableGameObject::getAttackHeight();
			void MovableGameObject::setAttackSize(Size size);
			void MovableGameObject::setAttackSize(float width, float height);

			void registerStateChangedCallback(std::function<void(MovableGameObject *gameObject)> stateChangedCallback);
			void registerHitCallback(std::function<void(MovableGameObject *gameObject)> hitCallback);

			virtual bool isAI() { return false; }
		private:
			Direction _spawnDirection;
			Size _attackSize;
			float _midAttackY, _longAttackY;
			std::vector<std::function<void(MovableGameObject *gameObject)>> _stateChangedCallbacks;
			std::vector<std::function<void(MovableGameObject *gameObject)>> _hitCallbacks;
			bool _shouldTurnArround;

			void triggerStateChangedCallbacks();
		protected:
			int _lives, _hp, _bp, _pp, _midCost, _longCost, _midDamage, _longDamage;
			b2Body *_attackBody = nullptr, *_shootBody = nullptr;
			Direction _direction;
			bool _isJumping, _isDead;
			Speed _speed;
			State _state;
			double _multiplierOnHit;
		};

		inline MovableGameObject::State operator|(MovableGameObject::State a, MovableGameObject::State b)
		{
			return static_cast<MovableGameObject::State>(static_cast<int>(a) | static_cast<int>(b));
		}

		inline MovableGameObject::State operator&(MovableGameObject::State a, MovableGameObject::State b)
		{
			return static_cast<MovableGameObject::State>(static_cast<int>(a)& static_cast<int>(b));
		}
	}
}