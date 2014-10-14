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
		MovablePlatform::MovablePlatform() { }

		MovablePlatform::~MovablePlatform() { }

		void MovablePlatform::checkDirectionChange()
		{
			if (_direction == Direction::RIGHT)
			{
				if (getPixelX() >= _endLocation.x)
				{
					setDirection(Direction::LEFT);
					//  _direction == Direction::LEFT;
					_body->SetLinearVelocity(b2Vec2(-_speed.horizontal, 0.0f));
				}
			}
			else if (_direction == Direction::LEFT)
			{
				if (getPixelX() <= _startLocation.x)
				{
					setDirection(Direction::RIGHT);
					//  _direction == Direction::RIGHT;
					_body->SetLinearVelocity(b2Vec2(_speed.horizontal, 0.0f));
				}
			}
			else if (_direction == Direction::DOWN)
			{
				if (getPixelY() >= _endLocation.y)
				{
					setDirection(Direction::UP);
					//  _direction == Direction::UP;
					_body->SetLinearVelocity(b2Vec2(0.0f, -_speed.vertical));
				}
			}
			else if (_direction == Direction::UP)
			{
				if (getPixelY() <= _startLocation.y)
				{
					setDirection(Direction::DOWN);
					//  _direction == Direction::DOWN;
					_body->SetLinearVelocity(b2Vec2(0.0f, _speed.vertical));
				}
			}
			/*
			else if (kinecticBody->GetDirection() == KinecticBody::Direction::Up)
			{
			if (tmp->GetPosition().y <= kinecticBody->getStartLocation()->y)
			{
			kinecticBody->setDirection(KinecticBody::Direction::Down);
			//tmp->SetLinearVelocity(b2Vec2(0.0, std::abs(tmp->GetLinearVelocity().y)));
			b2Vec2 vec = b2Vec2(b2Vec2(0.0, std::abs(tmp->GetLinearVelocity().y)));
			kinecticBody->changeBodiesLinearVelocity(vec);
			tmp->SetLinearVelocity(vec);
			}
			}
			*/
		}

		float  MovablePlatform::getStartLocationX()
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

		bool MovablePlatform::getDieOnImpact()
		{
			return _dieOnImpact;
		}

		void MovablePlatform::setDieOnImpact(bool isDieOnImpact)
		{
			_dieOnImpact = isDieOnImpact;
		}
	}
}