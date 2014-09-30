#pragma once
#include "Box2D\Box2D.h"

class Player
{
public:
	Player(float width, float height, float horizontalSpeed, float verticalSpeed);
	Player(float width, float height, float horizontalSpeed, float verticalSpeed, b2Body *body);
	virtual ~Player();

	void move(int direction);
	void jump();
	void setBody(b2Body *body);
	void setPlatformVelocityX(float platformVelocityX);
private:
	b2Body *_body;
	float _width, _height, _horizontalSpeed, _verticalSpeed, _platformVelocityX;
};