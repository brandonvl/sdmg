#include "Player.h"
#include "KinecticBody.h"
#include <iostream>

Player::Player(float width, float height, float horizontalSpeed, float verticalSpeed)
{
	_width = width;
	_height = height;
	_horizontalSpeed = horizontalSpeed;
	_verticalSpeed = verticalSpeed;
}

Player::Player(float width, float height, float horizontalSpeed, float verticalSpeed, b2Body *body)
{
	_width = width;
	_height = height;
	_horizontalSpeed = horizontalSpeed;
	_verticalSpeed = verticalSpeed;
	_body = body;
	_body->SetUserData(this);
}

Player::~Player(){}

void Player::setBody(b2Body *body)
{
	_body = body;
}

void Player::move(int direction)
{
	if (_platformVelocityX == 0.0f && direction == 0)
	{
		_body->SetLinearVelocity(b2Vec2(0.0f, _body->GetLinearVelocity().y));
	}
	else if (_platformVelocityX != 0.0f && direction == 0)
	{
		_body->SetLinearVelocity(b2Vec2(_platformVelocityX, 0.0f));
	}
	else if (_platformVelocityX == 0.0f && direction != 0)
	{
		_body->SetLinearVelocity(b2Vec2(_horizontalSpeed * direction, _body->GetLinearVelocity().y));

		/*
		b2Vec2 vel = _body->GetLinearVelocity();
		float desiredVel = _horizontalSpeed * direction;
		float velChange = desiredVel - vel.x;
		float impulse = _body->GetMass() * velChange; //disregard time factor

		_body->ApplyLinearImpulse(b2Vec2(impulse, 0), _body->GetWorldCenter(), true);
		*/
	}
	else if (_platformVelocityX != 0.0f && direction != 0)
	{
		_body->SetLinearVelocity(b2Vec2((_horizontalSpeed * direction) + _platformVelocityX, 0.0f));
	}
}

void Player::setPlatformVelocityX(float platformVelocityX)
{
  	_platformVelocityX = platformVelocityX;
}

void Player::jump()
{
	b2Vec2 jumpImpulse(0, -(_body->GetMass() * _verticalSpeed));
	//_body->ApplyLinearImpulse(jumpImpulse, _body->GetWorldCenter(), true);
	
	_body->SetLinearVelocity(jumpImpulse);
}