//
//
//  @ Project : SDMG
//  @ File Name : MovableGameObject.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "MovableGameObject.h"
#include "model/MovablePlatform.h"
#include "GameTime.h"
#include "GameBase.h"
#include "World.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"

#include <string>
#include <iostream>

namespace sdmg {
	namespace engine {
		MovableGameObject::MovableGameObject()
		{
			_state = State::IDLE;
		}

		MovableGameObject::~MovableGameObject()
		{
			_stateChangedCallbacks.clear();
			_hitCallbacks.clear();
		}

		void MovableGameObject::update(GameTime *gameTime, GameBase *game) {
			_speed = Speed(0.0f, 0.0f);
		}

		int MovableGameObject::getLives()
		{
			return _lives;
		}

		void MovableGameObject::setLives(int lives)
		{
			_lives = lives;
		}

		void MovableGameObject::hit(int damage)
		{
			setHP(_hp - damage);

			for (auto callback : _hitCallbacks)
				callback(this);
		}

		int MovableGameObject::getHP()
		{
			return _hp;
		}

		void MovableGameObject::setHP(int hp)
		{
			_hp = hp;
			if (_hp <= 0)
			{
				_hp = 0;
				_state = MovableGameObject::State::KNEELING;
			}
		}

		int MovableGameObject::getMidCost()
		{
			return _midCost;
		}

		void MovableGameObject::setMidCost(int cost)
		{
			_midCost = cost;
		}

		int MovableGameObject::getLongCost()
		{
			return _longCost;
		}

		void MovableGameObject::setLongCost(int cost)
		{
			_longCost = cost;
		}

		int MovableGameObject::getMidDamage()
		{
			return _midCost;
		}

		void MovableGameObject::setMidDamage(int damage)
		{
			_midDamage = damage;
		}

		int MovableGameObject::getLongDamage()
		{
			return _longCost;
		}

		void MovableGameObject::setLongDamage(int damage)
		{
			_longDamage = damage;
		}

		/*
		int MovableGameObject::getBP()
		{
			return _bp;
		}

		void MovableGameObject::setBP(int bp)
		{
			_bp = bp;
			if (_bp <= 0)
			{
				_bp = 0;
				_state = MovableGameObject::State::IDLE;
			}
		}
		*/

		int MovableGameObject::getPP()
		{
			return _pp;
		}

		void MovableGameObject::addPP(int pp)
		{
			_pp += pp;
			if (_pp <= 0)
				_pp = 0;
			else if (_pp < 10)
			{
				if (_state == MovableGameObject::State::BLOCKING || _state == (MovableGameObject::State::IDLE | MovableGameObject::State::BLOCKING))
					_state = MovableGameObject::State::IDLE;
			}
			else if (_pp > 100)
				_pp = 100;
		}

		void MovableGameObject::setPP(int pp)
		{
			_pp = pp;
			if (_pp <= 0)
				_pp = 0;
			else if (_pp > 100)
				_pp = 100;
		}

		Speed MovableGameObject::getSpeed()
		{
			return _speed;
		}

		bool MovableGameObject::getIsJumping()
		{
			return _isJumping;
		}

		void MovableGameObject::setIsJumping(bool isJumping)
		{
			_isJumping = isJumping;
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

		bool MovableGameObject::getShouldTurnArround()
		{
			return _shouldTurnArround;
		}

		void MovableGameObject::setShouldTurnArround(bool arround)
		{
			_shouldTurnArround = arround;
		}
		
		void MovableGameObject::stateCompleted() {
			/*
			switch (_state) {
			case State::KNOCKBACKLEFT:
			case State::KNOCKBACKRIGHT:
				_state = State::IDLE;
				break;
			case State::KNEELING:
				_state = State::RESPAWN;
				break;
			}
			*/

			if (_shouldTurnArround)
			{
				if (_direction == Direction::LEFT)
					_direction = Direction::RIGHT;
				else if (_direction == Direction::RIGHT)
					_direction = Direction::LEFT;
				_shouldTurnArround = false;
			}

			if (_state == State::KNEELING)
				_state = State::RESPAWN;
			else if (_state == State::KNOCKBACKLEFT || _state == State::KNOCKBACKRIGHT)
			{
				if (_isJumping)
					_state = State::FALLING;
				else
					_state = State::IDLE;
			}
			else if (_state == (State::IDLE | State::FORWARD_ROLL))
				_state = State::IDLE;
			else if (_state == (State::WALKING | State::FORWARD_ROLL))
				_state = State::WALKING;
			else if (_state == (State::JUMPING | State::FORWARD_ROLL))
			{
				if (_body->GetLinearVelocity().y < 0)
					_state = State::JUMPING;
				else if (_body->GetLinearVelocity().y > 0)
					_state = State::FALLING;
				else
					_state = State::IDLE;
			}
			else if (_state == (State::JUMPINGLEFT | State::FORWARD_ROLL))
			{
				_direction = Direction::LEFT;

				if (_body->GetLinearVelocity().y < 0)
					_state = State::JUMPINGLEFT;
				else if (_body->GetLinearVelocity().y > 0)
					_state = State::FALLINGLEFT;
				else
					_state = State::IDLE;
			}
			else if (_state == (State::JUMPINGRIGHT | State::FORWARD_ROLL))
			{
				_direction = Direction::RIGHT;

				if (_body->GetLinearVelocity().y < 0)
					_state = State::JUMPINGRIGHT;
				else if (_body->GetLinearVelocity().y > 0)
					_state = State::FALLINGRIGHT;
				else
					_state = State::IDLE;
			}
			else if (_state == (State::FALLING | State::FORWARD_ROLL))
			{
				if (_body->GetLinearVelocity().y < 0)
					_state = State::JUMPING;
				else if (_body->GetLinearVelocity().y > 0)
					_state = State::FALLING;
				else
					_state = State::IDLE;
			}
			else if (_state == (State::FALLINGLEFT | State::FORWARD_ROLL))
			{
				_direction = Direction::LEFT;

				if (_body->GetLinearVelocity().y < 0)
					_state = State::JUMPINGLEFT;
				else if (_body->GetLinearVelocity().y > 0)
					_state = State::FALLINGLEFT;
				else
					_state = State::WALKING;
			}
			else if (_state == (State::FALLINGRIGHT | State::FORWARD_ROLL))
			{
				// _direction = Direction::RIGHT;

				if (_body->GetLinearVelocity().y < 0)
					_state = State::JUMPINGRIGHT;
				else if (_body->GetLinearVelocity().y > 0)
					_state = State::FALLINGRIGHT;
				else
					_state = State::WALKING;
			}

			else if (_state == (State::WALKING | State::MIDRANGEATTACKBEGIN))
				_state = State::WALKING | State::MIDRANGEATTACK;
			else if (_state == (State::WALKING | State::MIDRANGEATTACK))
				_state = State::WALKING | State::MIDRANGEATTACKEND;
			else if (_state == (State::WALKING | State::MIDRANGEATTACKEND)){
				if (_body->GetLinearVelocity().y > 1.0)
				{
					if (_direction == Direction::LEFT)
						_state = State::FALLINGLEFT;
					else if (_direction == Direction::RIGHT)
						_state = State::FALLINGRIGHT;
					else
						_state = State::WALKING;
				}
				else if (_body->GetLinearVelocity().y < -1.0)
				{
					if (_direction == Direction::LEFT)
						_state = State::JUMPINGLEFT;
					else if (_direction == Direction::RIGHT)
						_state = State::JUMPINGRIGHT;
					else
						_state = State::WALKING;
				}
				else
					_state = State::WALKING;
			}

			else if (_state == (State::IDLE | State::MIDRANGEATTACKBEGIN))
				_state = State::IDLE | State::MIDRANGEATTACK;
			else if (_state == (State::IDLE | State::MIDRANGEATTACK))
				_state = State::IDLE | State::MIDRANGEATTACKEND;
			else if (_state == (State::IDLE | State::MIDRANGEATTACKEND))
			{
				if (_body->GetLinearVelocity().y > 1.0)
					_state = State::FALLING;
				else if (_body->GetLinearVelocity().y < -1.0)
					_state = State::JUMPING;
				else
					_state = State::IDLE;
			}

			else if (_state == (State::WALKING | State::LONGRANGEATTACKBEGIN))
				_state = State::WALKING | State::LONGRANGEATTACK;
			else if (_state == (State::WALKING | State::LONGRANGEATTACK))
				_state = State::WALKING | State::LONGRANGEATTACKEND;
			else if (_state == (State::WALKING | State::LONGRANGEATTACKEND))
				_state = State::WALKING;

			else if (_state == (State::IDLE | State::LONGRANGEATTACKBEGIN))
				_state = State::IDLE | State::LONGRANGEATTACK;
			else if (_state == (State::IDLE | State::LONGRANGEATTACK))
				_state = State::IDLE | State::LONGRANGEATTACKEND;
			else if (_state == (State::IDLE | State::LONGRANGEATTACKEND))
				_state = State::IDLE;
		}

		bool MovableGameObject::stateIsInterruptible()
		{
			/*
			if (_state == State::FORWARD_ROLL || _state == State::KNOCKBACKLEFT || _state == State::KNEELING || _state == State::KNOCKBACKRIGHT
			|| _state == State::MIDRANGEATTACKBEGIN || _state == State::MIDRANGEATTACK || _state == State::MIDRANGEATTACKEND ||
			_state == State::LONGRANGEATTACK)
			*/
			if (_state == State::KNEELING || _state == State::KNOCKBACKLEFT || _state == State::KNOCKBACKRIGHT || _state == State::RESPAWN ||
				_state == (State::WALKING | State::MIDRANGEATTACKBEGIN) || _state == (State::WALKING | State::MIDRANGEATTACK)
				|| _state == (State::WALKING | State::MIDRANGEATTACKEND) || _state == (State::IDLE | State::MIDRANGEATTACKBEGIN)
				|| _state == (State::IDLE | State::MIDRANGEATTACK) || _state == (State::IDLE | State::MIDRANGEATTACKEND)
				|| _state == (State::WALKING | State::LONGRANGEATTACKBEGIN) || _state == (State::WALKING | State::LONGRANGEATTACK)
				|| _state == (State::WALKING | State::LONGRANGEATTACKEND) || _state == (State::IDLE | State::LONGRANGEATTACKBEGIN)
				|| _state == (State::IDLE | State::LONGRANGEATTACK) || _state == (State::IDLE | State::LONGRANGEATTACKEND)
				|| _state == (State::IDLE | State::FORWARD_ROLL) || _state == (State::WALKING | State::FORWARD_ROLL)
				|| _state == (State::JUMPING | State::FORWARD_ROLL) || _state == (State::JUMPINGLEFT | State::FORWARD_ROLL)
				|| _state == (State::JUMPINGRIGHT | State::FORWARD_ROLL) || _state == (State::FALLING | State::FORWARD_ROLL)
				|| _state == (State::FALLINGLEFT | State::FORWARD_ROLL) || _state == (State::FALLINGRIGHT | State::FORWARD_ROLL)
				)
				return false;
			return true;
		}

		MovableGameObject::Direction MovableGameObject::getSpawnDirection()
		{
			return _spawnDirection;
		}

		void MovableGameObject::setSpawnDirection(Direction direction)
		{
			_spawnDirection = direction;
		}

		b2Body* MovableGameObject::getAttackBody()
		{
			return _attackBody;
		}

		void MovableGameObject::setAttackBody(b2Body *attackBody)
		{
			_attackBody = attackBody;
		}

		b2Body* MovableGameObject::getShootBody()
		{
			return _shootBody;
		}

		void MovableGameObject::setShootBody(b2Body *shootBody)
		{
			_shootBody = shootBody;
		}

		void MovableGameObject::destroyAttackBody()
		{
			if (_attackBody != nullptr)
			{
				delete static_cast<model::MovablePlatform*>(_attackBody->GetUserData());
				_attackBody->GetWorld()->DestroyBody(_attackBody);
				_attackBody = nullptr;
			}
		}

		void MovableGameObject::destroyShootBody(Engine &engine)
		{
			if (_shootBody != nullptr)
			{
				model::MovablePlatform *platform = static_cast<model::MovablePlatform*>(_shootBody->GetUserData());
				getWorld()->removePlatform(platform);
				engine.getDrawEngine()->unloadCopy(platform);
				delete platform;
				_shootBody->GetWorld()->DestroyBody(_shootBody);
				_shootBody = nullptr;
			}
		}

		MovableGameObject::State MovableGameObject::getState() { return _state; }
		void MovableGameObject::setState(State state)
		{
			// check if state is changed
			if (state != _state) {
				//if (stateIsInterruptible())
				_state = state;

				if (_state != (State::WALKING | State::MIDRANGEATTACKBEGIN) && _state != (State::WALKING | State::MIDRANGEATTACK)
					&& _state != (State::WALKING | State::MIDRANGEATTACKEND) && _state != (State::IDLE | State::MIDRANGEATTACKBEGIN)
					&& _state != (State::IDLE | State::MIDRANGEATTACK) && _state != (State::IDLE | State::MIDRANGEATTACKEND)
					&& _state != (State::WALKING | State::LONGRANGEATTACKBEGIN) && _state != (State::WALKING | State::LONGRANGEATTACK)
					&& _state != (State::WALKING | State::LONGRANGEATTACKEND) && _state != (State::IDLE | State::LONGRANGEATTACKBEGIN)
					&& _state != (State::IDLE | State::LONGRANGEATTACK) && _state != (State::IDLE | State::LONGRANGEATTACKEND)
					&& _state != (State::IDLE | State::FORWARD_ROLL) && _state != (State::WALKING | State::FORWARD_ROLL)
					&& _state != (State::JUMPING | State::FORWARD_ROLL) && _state != (State::JUMPINGLEFT | State::FORWARD_ROLL)
					&& _state != (State::JUMPINGRIGHT | State::FORWARD_ROLL) && _state != (State::FALLING | State::FORWARD_ROLL)
					&& _state != (State::FALLINGLEFT | State::FORWARD_ROLL) && _state != (State::FALLINGRIGHT | State::FORWARD_ROLL)
					)
				triggerStateChangedCallbacks();
			}
		}

		bool MovableGameObject::getRolling()
		{
			return _state == (State::IDLE | State::FORWARD_ROLL) || _state == (State::WALKING | State::FORWARD_ROLL)
				|| _state == (State::JUMPING | State::FORWARD_ROLL) || _state == (State::JUMPINGLEFT | State::FORWARD_ROLL)
				|| _state == (State::JUMPINGRIGHT | State::FORWARD_ROLL) || _state == (State::FALLING | State::FORWARD_ROLL)
				|| _state == (State::FALLINGLEFT | State::FORWARD_ROLL) || _state == (State::FALLINGRIGHT | State::FORWARD_ROLL);
		}

		bool MovableGameObject::getFalling()
		{
			return _state == State::FALLING || _state == State::FALLINGLEFT || _state == State::FALLINGRIGHT;
		}

		MovableGameObject::Direction MovableGameObject::getDirection() { return _direction; }
		void MovableGameObject::setDirection(Direction direction) { _direction = direction; }

		float MovableGameObject::getAttackWidth()
		{
			return _attackSize.width;
		}

		float MovableGameObject::getAttackHeight()
		{
			return _attackSize.height;
		}

		void MovableGameObject::setAttackSize(Size size)
		{
			_attackSize = size;
		}

		void MovableGameObject::setAttackSize(float width, float height)
		{
			_attackSize = Size(width, height);
		}

		float MovableGameObject::getMidAttackY()
		{
			return _midAttackY;
		}

		void MovableGameObject::setMidAttackY(float y)
		{
			_midAttackY = y;
		}

		float MovableGameObject::getLongAttackY()
		{
			return _longAttackY;
		}

		void MovableGameObject::setLongAttackY(float y)
		{
			_longAttackY = y;
		}

		void MovableGameObject::registerStateChangedCallback(std::function<void(MovableGameObject *gameObject)> stateChangedCallback) {
			_stateChangedCallbacks.push_back(stateChangedCallback);
		}

		void MovableGameObject::registerHitCallback(std::function<void(MovableGameObject *gameObject)> hitCallback) {
			_hitCallbacks.push_back(hitCallback);
		}

		void MovableGameObject::triggerStateChangedCallbacks() {
			for (auto callback : _stateChangedCallbacks) {
				callback(this);
			}
		}
	}
}
