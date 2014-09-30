#pragma once
#include "Box2D\Box2D.h"
#include <vector>

class KinecticBody
{
public:
	KinecticBody();
	KinecticBody(b2Vec2 *startLocation);
	virtual ~KinecticBody();

	enum class Direction { Up, Left, Down, Right, None };

	void setStartLocation(b2Vec2 *vec);
	void setEndLocation(b2Vec2 *vec);
	void setDirection(Direction direction);
	void addBody(b2Body *body, b2Vec2 velocity);
	void removeBody(b2Body *body);
	void changeBodiesLinearVelocity(b2Vec2 velocity);

	b2Vec2* getStartLocation();
	b2Vec2* getEndLocation();
	Direction GetDirection();

private:
	b2Vec2 *_startLocation, *_endLocation;
	KinecticBody::Direction _direction;
	std::vector<b2Body*> *_bodies;
};

