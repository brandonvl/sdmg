//
//
//  @ Project : SDMG
//  @ File Name : MovablePlatform.cpp
//  @ Date : 2-10-2014
//  @ Author : 42IN13SAd
//
//

#include "MovablePlatform.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace model {
		MovablePlatform::MovablePlatform()
		{
			_damageOnImpact = 0;
			_mustBeDestroyed = false;
			_moveing = Moveing::REPEAT;
		}

		MovablePlatform::~MovablePlatform() { }

		void MovablePlatform::checkDirectionChange()
		{
			if (_startLocation.x < _endLocation.x || _startLocation.y < _endLocation.y)
				checkEndHigherThanStart();
			else if (_startLocation.x > _endLocation.x || _startLocation.y > _endLocation.y)
				checkStartHigherThanEnd();
		}

		void MovablePlatform::checkEndHigherThanStart()
		{
			if (_direction == Direction::RIGHT)
			{
				float32 h = getPixelX();
				if (getPixelX() >= _endLocation.x)
				{
					_body->SetLinearVelocity(b2Vec2(-_speed.horizontal, 0.0f));
					changeDirection(Direction::LEFT);
				}
			}
			else if (_direction == Direction::LEFT)
			{
				float32 h = getPixelX();
				if (getPixelX() <= _startLocation.x)
				{
					_body->SetLinearVelocity(b2Vec2(_speed.horizontal, 0.0f));
					changeDirection(Direction::RIGHT);
				}
			}
			else if (_direction == Direction::DOWN)
			{
				if (getPixelY() >= _endLocation.y)
				{
					_body->SetLinearVelocity(b2Vec2(0.0f, -_speed.vertical));
					changeDirection(Direction::UP);
				}
			}
			else if (_direction == Direction::UP)
			{
				if (getPixelY() <= _startLocation.y)
				{
					_body->SetLinearVelocity(b2Vec2(0.0f, _speed.vertical));
					changeDirection(Direction::DOWN);
				}
			}
		}

		void MovablePlatform::checkStartHigherThanEnd()
		{
			if (_direction == Direction::RIGHT)
			{
				float32 h = getPixelX();
				if (getPixelX() >= _startLocation.x)
				{
					_body->SetLinearVelocity(b2Vec2(-_speed.horizontal, 0.0f));
					changeDirection(Direction::LEFT);
				}
			}
			else if (_direction == Direction::LEFT)
			{
				float32 h = getPixelX();
				if (getPixelX() <= _endLocation.x)
				{
					_body->SetLinearVelocity(b2Vec2(_speed.horizontal, 0.0f));
					changeDirection(Direction::RIGHT);
				}
			}
			else if (_direction == Direction::DOWN)
			{
				if (getPixelY() >= _startLocation.y)
				{
					_body->SetLinearVelocity(b2Vec2(0.0f, -_speed.vertical));
					changeDirection(Direction::UP);
				}
			}
			else if (_direction == Direction::UP)
			{
				if (getPixelY() <= _endLocation.y)
				{
					_body->SetLinearVelocity(b2Vec2(0.0f, _speed.vertical));
					changeDirection(Direction::DOWN);
				}
			}
		}

		void MovablePlatform::changeDirection(MovableGameObject::Direction direction)
		{
			if (_moveing == Moveing::REPEAT)
				setDirection(direction);
			else if (_moveing == Moveing::ONCE)
				_mustBeDestroyed = true;
		}

		float MovablePlatform::getStartLocationX()
		{
			return _startLocation.x;
		}

		float  MovablePlatform::getStartLocationY()
		{
			return _startLocation.y;
		}

		float  MovablePlatform::getEndLocationX()
		{
			return _endLocation.x;
		}

		float  MovablePlatform::getEndLocationY()
		{
			return _endLocation.y;
		}

		void MovablePlatform::setStartLocation(b2Vec2 vec)
		{
			_startLocation = vec;
		}

		void MovablePlatform::setEndLocation(b2Vec2 vec)
		{
			_endLocation = vec;
		}

		int MovablePlatform::getDamageOnImpact()
		{
			return _damageOnImpact;
		}

		void MovablePlatform::setDamageOnImpact(int damage)
		{
			_damageOnImpact = damage;
		}

		MovablePlatform::Moveing MovablePlatform::getMoveing()
		{
			return _moveing;
		}

		void MovablePlatform::setMoveing(Moveing moveing)
		{
			_moveing = moveing;
		}

		MovableGameObject *MovablePlatform::getOwner()
		{
			return _owner;
		}

		void MovablePlatform::setOwner(MovableGameObject *owner)
		{
			_owner = owner;
		}

		bool MovablePlatform::getMustBeDestroyed()
		{
			return _mustBeDestroyed;
		}

		void MovablePlatform::setMustBeDestroyed(bool destroy)
		{
			_mustBeDestroyed = destroy;
		}
	}
}