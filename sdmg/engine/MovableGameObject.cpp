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

		int MovableGameObject::getHP()
		{
			return _hp;
		}

		void MovableGameObject::setHP(int hp)
		{
			_hp = hp;
			if (_hp <= 0)
				_state = MovableGameObject::State::KNEELING;
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

		void MovableGameObject::stateCompleted() {
			switch (_state) {
			case State::FORWARD_ROLL:
			case State::KNOCKBACKLEFT:
			case State::KNOCKBACKRIGHT:
			case State::SHORTRANGEATTACK:
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
		}

		bool MovableGameObject ::stateIsInterruptible()
		{
			if (_state == State::FORWARD_ROLL || _state == State::KNOCKBACKLEFT || _state == State::KNEELING || _state == State::KNOCKBACKRIGHT
				|| _state == State::SHORTRANGEATTACK ||
				_state == State::MIDRANGEATTACKBEGIN || _state == State::MIDRANGEATTACK || _state == State::MIDRANGEATTACKEND
				|| _state == State::LONGRANGEATTACK)
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

		MovableGameObject::State MovableGameObject::getState() { return _state; }
		void MovableGameObject::setState(State state)
		{
			// check if state is changed
			if (state != _state) {
				//if (stateIsInterruptible())
				_state = state;
				triggerStateChangedCallbacks();
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

		float MovableGameObject::getAttackY()
		{
			return _attackY;
		}

		void MovableGameObject::setAttackY(float y)
		{
			_attackY = y;
		}

		void MovableGameObject::registerStateChangedCallback(std::function<void(MovableGameObject *gameObject)> stateChangedCallback) {
			_stateChangedCallbacks.push_back(stateChangedCallback);
		}

		void MovableGameObject::triggerStateChangedCallbacks() {
			for (auto callback : _stateChangedCallbacks) {
				callback(this);
			}
		}
	}
}
