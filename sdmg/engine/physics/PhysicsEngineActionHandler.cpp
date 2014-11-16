//
//
//  @ Project : SDMG
//  @ File Name : PhysicsEngine.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#include "PhysicsEngineActionHandler.h"
#include "engine\MovableGameObject.h"
#include "model\Platform.h"
#include "model\MovablePlatform.h"
#include <iostream>

namespace sdmg {
	namespace engine {
		namespace physics {
			PhysicsEngineActionHandler::PhysicsEngineActionHandler()
			{
			}


			PhysicsEngineActionHandler::~PhysicsEngineActionHandler()
			{

			}


			void PhysicsEngineActionHandler::moveLeft(MovableGameObject *obj) {
				obj->getBody()->SetLinearVelocity(b2Vec2(obj->getHorizontalSpeed() * -1, obj->getBody()->GetLinearVelocity().y));

			}

			void PhysicsEngineActionHandler::moveRight(MovableGameObject *obj) {
				obj->getBody()->SetLinearVelocity(b2Vec2(obj->getHorizontalSpeed(), obj->getBody()->GetLinearVelocity().y));
			}

			void PhysicsEngineActionHandler::idle(MovableGameObject *obj) {
				obj->getBody()->SetLinearVelocity(b2Vec2(0.0f, obj->getBody()->GetLinearVelocity().y));
			}

			void PhysicsEngineActionHandler::jump(MovableGameObject *obj) {
				b2Vec2 jumpImpulse(obj->getBody()->GetLinearVelocity().x, -(obj->getBody()->GetMass() * obj->getVerticalSpeed()));
				//_body->ApplyLinearImpulse(jumpImpulse, _body->GetWorldCenter(), true);

				obj->getBody()->SetLinearVelocity(jumpImpulse);
			}

			void PhysicsEngineActionHandler::kneel(MovableGameObject *obj)
			{
				if (obj->getAttackBody() != nullptr)
					midRangeAttackEnd(obj);
				obj->getBody()->SetLinearVelocity(b2Vec2(0.0f, obj->getBody()->GetLinearVelocity().y));
			}

			void PhysicsEngineActionHandler::midRangeAttackBegin(MovableGameObject *obj) {
				//  obj->getBody()->SetLinearVelocity(b2Vec2(0.0f, obj->getBody()->GetLinearVelocity().y));
				obj->getBody()->SetLinearVelocity(b2Vec2(obj->getBody()->GetLinearVelocity().x, obj->getBody()->GetLinearVelocity().y));
			}

			void PhysicsEngineActionHandler::midRangeAttack(MovableGameObject *obj) {
				//  obj->getBody()->SetLinearVelocity(b2Vec2(0.0f, obj->getBody()->GetLinearVelocity().y));
				obj->getBody()->SetLinearVelocity(b2Vec2(obj->getBody()->GetLinearVelocity().x, obj->getBody()->GetLinearVelocity().y));

				float _P2M = 1.0f / 20.0f;
				if (obj->getAttackBody() == nullptr)
				{
					b2BodyDef *bodydef = new b2BodyDef();


					model::Platform *platform = new model::Platform(true);

					if (obj->getDirection() == MovableGameObject::Direction::LEFT)
						bodydef->position.Set(obj->getX() - (_P2M * 30), obj->getY() + ((obj->getHeight() * _P2M) / 2) - obj->getAttackY());
					else if (obj->getDirection() == MovableGameObject::Direction::RIGHT)
						bodydef->position.Set(obj->getX() + (obj->getWidth() * _P2M) - 4, obj->getY() + ((obj->getHeight() * _P2M) / 2) - obj->getAttackY());

					//  bodydef->type = b2_staticBody;
					b2Body *body = obj->getBody()->GetWorld()->CreateBody(bodydef);

					body->SetFixedRotation(true);
					// bodydef->userData = &body;

					b2PolygonShape *shape = new b2PolygonShape();

					shape->SetAsBox(_P2M * obj->getAttackWidth(), _P2M * obj->getAttackHeight());

					b2FixtureDef *fixturedef = new b2FixtureDef();
					fixturedef->shape = shape;
					fixturedef->density = 1.0f;
					body->CreateFixture(fixturedef);

					
					body->SetUserData(platform);
					obj->setAttackBody(body);

					platform->setLocation(&body->GetPosition().x, &body->GetPosition().y);
					
					delete fixturedef;
					delete shape;
					delete bodydef;
				}
				else
				{
					if (obj->getDirection() == MovableGameObject::Direction::LEFT)
					{
						obj->getAttackBody()->SetTransform(b2Vec2(obj->getX() - (_P2M * 30), obj->getY() + ((obj->getHeight() * _P2M) / 2) - obj->getAttackY()), obj->getAttackBody()->GetAngle());
					}
					else if (obj->getDirection() == MovableGameObject::Direction::RIGHT)
					{
						obj->getAttackBody()->SetTransform(b2Vec2(obj->getX() + (obj->getWidth() * _P2M) - 4, obj->getY() + ((obj->getHeight() * _P2M) / 2) - obj->getAttackY()), obj->getAttackBody()->GetAngle());
					}
				}
			}

			void PhysicsEngineActionHandler::midRangeAttackEnd(MovableGameObject *obj) {
				//  obj->getBody()->SetLinearVelocity(b2Vec2(obj->getBody()->GetLinearVelocity().x, obj->getBody()->GetLinearVelocity().y));
				obj->getBody()->SetLinearVelocity(b2Vec2(obj->getBody()->GetLinearVelocity().x, obj->getBody()->GetLinearVelocity().y));

				if (obj->getAttackBody() != nullptr &&! obj->getBody()->GetWorld()->IsLocked())
				{
					delete static_cast<sdmg::model::Platform*>(obj->getAttackBody()->GetUserData());
					obj->getBody()->GetWorld()->DestroyBody(obj->getAttackBody());
					obj->setAttackBody(nullptr);
				}
			}

			void PhysicsEngineActionHandler::knockbackLeft(MovableGameObject *obj) {
				if (obj->getAttackBody() != nullptr)
					midRangeAttackEnd(obj);
				obj->getBody()->SetLinearVelocity(b2Vec2(50.0f * -1, -2.0f));
			}

			void PhysicsEngineActionHandler::knockbackRight(MovableGameObject *obj) {
				if (obj->getAttackBody() != nullptr)
					midRangeAttackEnd(obj);
				obj->getBody()->SetLinearVelocity(b2Vec2(50.0f, -2.0f));
			}

			void PhysicsEngineActionHandler::respawn(MovableGameObject *obj)
			{
				if (obj->getAttackBody() != nullptr)
					midRangeAttackEnd(obj);
				obj->setHP(100);
				obj->setBP(100);
				obj->setPP(100);
				obj->setLives(obj->getLives() - 1);

				if (obj->getLives() == 0)
					obj->die();

				obj->setDirection(obj->getSpawnDirection());
				obj->setState(MovableGameObject::State::FALLING);
				obj->getBody()->SetTransform(b2Vec2(obj->getSpawnLocationX() / 20.0f, obj->getSpawnLocationY() / 20.0f), obj->getBody()->GetAngle());
				obj->getBody()->SetLinearVelocity(b2Vec2(0.0f, -1.0f));
			}

			void PhysicsEngineActionHandler::longRangeAttackBegin(MovableGameObject *obj)
			{
				obj->getBody()->SetLinearVelocity(b2Vec2(obj->getBody()->GetLinearVelocity().x, obj->getBody()->GetLinearVelocity().y));
			}

			void PhysicsEngineActionHandler::longRangeAttack(MovableGameObject *obj)
			{//  obj->getBody()->SetLinearVelocity(b2Vec2(0.0f, obj->getBody()->GetLinearVelocity().y));
				obj->getBody()->SetLinearVelocity(b2Vec2(obj->getBody()->GetLinearVelocity().x, obj->getBody()->GetLinearVelocity().y));

				float _P2M = 1.0f / 20.0f;
				if (obj->getShootBody() == nullptr)
				{
					model::MovablePlatform *platform = new model::MovablePlatform();
					platform->setSize(20, 20);
					platform->setSpeed(50.0f, 0.0f);
					platform->setDamageOnImpact(20);
					platform->setMoveing(model::MovablePlatform::Moveing::ONCE);
					platform->setOwner(obj);

					b2BodyDef *bodydef = new b2BodyDef();
					b2Vec2 location, velocity;

					if (obj->getDirection() == MovableGameObject::Direction::LEFT)
					{
						location = b2Vec2(obj->getX() - (_P2M * 30), obj->getY() + ((obj->getHeight() * _P2M) / 2) - obj->getAttackY());
						bodydef->position.Set(location.x, location.y);
						platform->setLocation(location.x, location.y);
						platform->setStartLocation(b2Vec2(obj->getPixelX() - 30, obj->getPixelY() + (obj->getHeight() / 2) - obj->getAttackY()));
						platform->setEndLocation(b2Vec2(-100, obj->getPixelY() + (obj->getHeight() / 2) - obj->getAttackY()));
						platform->setDirection(MovableGameObject::Direction::LEFT);

						velocity = b2Vec2(-platform->getSpeed().horizontal, platform->getSpeed().vertical);
					}
					else if (obj->getDirection() == MovableGameObject::Direction::RIGHT)
					{
						location = b2Vec2(obj->getX() + (obj->getWidth() * _P2M) - 4, obj->getY() + ((obj->getHeight() * _P2M) / 2) - obj->getAttackY());
						bodydef->position.Set(location.x, location.y);
						platform->setLocation(location.x, location.y);
						platform->setStartLocation(b2Vec2(obj->getPixelX() + obj->getWidth(), obj->getPixelY() + (obj->getHeight() / 2) - obj->getAttackY()));
						platform->setEndLocation(b2Vec2(1380, obj->getPixelY() + (obj->getHeight() / 2) - obj->getAttackY()));
						platform->setDirection(MovableGameObject::Direction::RIGHT);

						velocity = b2Vec2(platform->getSpeed().horizontal, platform->getSpeed().vertical);
					}


					bodydef->type = b2_kinematicBody;
					b2Body *body = obj->getBody()->GetWorld()->CreateBody(bodydef);

					body->SetFixedRotation(true);
					body->SetLinearVelocity(velocity);
					// bodydef->userData = &body;

					b2PolygonShape *shape = new b2PolygonShape();

					shape->SetAsBox(_P2M * 20, _P2M * 20);

					b2FixtureDef *fixturedef = new b2FixtureDef();
					fixturedef->shape = shape;
					fixturedef->density = 1.0f;
					body->CreateFixture(fixturedef);

					body->SetUserData(platform);
					obj->setShootBody(body);
					platform->setBody(body);

					platform->setLocation(&body->GetPosition().x, &body->GetPosition().y);

					delete fixturedef;
					delete shape;
					delete bodydef;
				}
			}

			void PhysicsEngineActionHandler::longRangeAttackEnd(MovableGameObject *obj)
			{
				obj->getBody()->SetLinearVelocity(b2Vec2(obj->getBody()->GetLinearVelocity().x, obj->getBody()->GetLinearVelocity().y));
			}
		}
	}
}