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

#include <string>
#include <iostream>

namespace sdmg {
	namespace engine {
		MovableGameObject::MovableGameObject()
		{
			_state = State::IDLE;
		}

		MovableGameObject::~MovableGameObject() { }

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
			switch (_state) {
			case State::FORWARD_ROLL:
			case State::KNOCKBACKLEFT:
			case State::KNOCKBACKRIGHT:
			case State::LONGRANGEATTACK:
			case State::MIDRANGEATTACKEND:
				_state = State::IDLE;
				break;
			case State::KNEELING:
				_state = State::RESPAWN;
				break;
			case State::MIDRANGEATTACKBEGIN:
				_state = State::MIDRANGEATTACK;
				break;
			case State::MIDRANGEATTACK:
				_state = State::MIDRANGEATTACKEND;
				break;
			}

			if (_shouldTurnArround)
			{
				if (_direction == Direction::LEFT)
					_direction = Direction::RIGHT;
				else if (_direction == Direction::RIGHT)
					_direction = Direction::LEFT;
				_shouldTurnArround = false;
			}

			if (_state == (State::IDLE | State::FORWARD_ROLL))
				_state = State::IDLE;
			else if (_state == (State::WALKING | State::FORWARD_ROLL))
				_state = State::WALKING;
			else if(_state == (State::JUMPING | State::FORWARD_ROLL))
				_state = State::JUMPING;
			else if (_state == (State::JUMPINGLEFT | State::FORWARD_ROLL))
			{
				_direction = Direction::LEFT;
				_state = State::JUMPINGLEFT;
			}
			else if (_state == (State::JUMPINGRIGHT | State::FORWARD_ROLL))
			{
				_direction = Direction::RIGHT;
				_state = State::JUMPINGRIGHT;
			}
			else if(_state == (State::FALLING | State::FORWARD_ROLL))
				_state = State::FALLING;
			else if (_state == (State::FALLINGLEFT | State::FORWARD_ROLL))
			{
				_direction = Direction::LEFT;
				_state = State::FALLINGLEFT;
			}
			else if (_state == (State::FALLINGRIGHT | State::FORWARD_ROLL))
			{
				_direction = Direction::RIGHT;
				_state = State::FALLINGRIGHT;
			}

			else if (_state == (State::WALKING | State::MIDRANGEATTACKBEGIN))
				_state = State::WALKING | State::MIDRANGEATTACK;
			else if (_state == (State::WALKING | State::MIDRANGEATTACK))
				_state = State::WALKING | State::MIDRANGEATTACKEND;
			else if (_state == (State::WALKING | State::MIDRANGEATTACKEND))
				_state = State::WALKING;

			else if (_state == (State::IDLE | State::MIDRANGEATTACKBEGIN))
				_state = State::IDLE | State::MIDRANGEATTACK;
			else if (_state == (State::IDLE | State::MIDRANGEATTACK))
				_state = State::IDLE | State::MIDRANGEATTACKEND;
			else if (_state == (State::IDLE | State::MIDRANGEATTACKEND))
				_state = State::IDLE;

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
			if (_state == State::KNEELING || _state == State::KNOCKBACKLEFT || _state == State::KNOCKBACKRIGHT ||
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

		void MovableGameObject::destroyShootBody()
		{
			if (_shootBody != nullptr)
			{
					delete static_cast<model::MovablePlatform*>(_shootBody->GetUserData());
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

				_state = state;
			}
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
