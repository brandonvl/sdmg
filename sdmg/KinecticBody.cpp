#include "KinecticBody.h"
#include "Box2D/Box2D.h"
#include "Player.h"

KinecticBody::KinecticBody()
{
	_bodies = new std::vector < b2Body*>();
}

KinecticBody::KinecticBody(b2Vec2 *startLocation)
{
	_bodies = new std::vector < b2Body*>();
	_startLocation = startLocation;
}

KinecticBody::~KinecticBody()
{
}

void KinecticBody::setStartLocation(b2Vec2 *vec)
{
	_startLocation = vec;
}
void KinecticBody::setEndLocation(b2Vec2 *vec)
{
	_endLocation = vec;
}

void KinecticBody::setDirection(KinecticBody::Direction direction)
{
	_direction = direction;
}

void KinecticBody::addBody(b2Body *body, b2Vec2 velocity)
{
	if (body->GetType() == b2_dynamicBody)
	{
		_bodies->push_back(body);
		Player *player = static_cast<Player*>(body->GetUserData());
		player->setPlatformVelocityX(velocity.x);
	}
}

void KinecticBody::removeBody(b2Body *body)
{
	//_bodies->
	_bodies->erase(std::remove(_bodies->begin(), _bodies->end(), body), _bodies->end());
	Player *player = static_cast<Player*>(body->GetUserData());
	player->setPlatformVelocityX(0.0f);
}

void KinecticBody::changeBodiesLinearVelocity(b2Vec2 velocity)
{
	for (auto i = _bodies->begin(); i != _bodies->end(); i++)
	{
		(*i)->SetLinearVelocity(velocity);
		Player *player = static_cast<Player*>((*i)->GetUserData());
		player->setPlatformVelocityX(velocity.x);
	}
}

b2Vec2* KinecticBody::getStartLocation()
{
	return _startLocation;
}

b2Vec2* KinecticBody::getEndLocation()
{
	return _endLocation;
}

KinecticBody::Direction KinecticBody::GetDirection()
{
	return _direction;
}
