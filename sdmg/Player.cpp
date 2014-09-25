#include "Player.h"

b2Body *_body;
float _width, _height, _horizontalSpeed, _verticalSpeed;

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
}

Player::~Player(){}

void Player::setBody(b2Body *body)
{
	_body = body;
}

void Player::move(int direction)
{
	b2Vec2 vel = _body->GetLinearVelocity();
	float desiredVel = _horizontalSpeed * direction;
	float velChange = desiredVel - vel.x;
	float impulse = _body->GetMass() * velChange; //disregard time factor
	_body->ApplyLinearImpulse(b2Vec2(impulse, 0), _body->GetWorldCenter(), true);
}

void Player::jump()
{
	b2Vec2 jumpImpulse(0, -(_body->GetMass() * _verticalSpeed));
	//_body->ApplyLinearImpulse(jumpImpulse, _body->GetWorldCenter(), true);
	
	_body->SetLinearVelocity(jumpImpulse);
}