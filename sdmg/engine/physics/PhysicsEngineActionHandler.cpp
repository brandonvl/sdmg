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

			void PhysicsEngineActionHandler::shortRangeAttack(MovableGameObject *obj) {

			}

			void PhysicsEngineActionHandler::midRangeAttackBegin(MovableGameObject *obj) {
				obj->getBody()->SetLinearVelocity(b2Vec2(0.0f, obj->getBody()->GetLinearVelocity().y));
			}

			void PhysicsEngineActionHandler::midRangeAttack(MovableGameObject *obj) {
				obj->getBody()->SetLinearVelocity(b2Vec2(0.0f, obj->getBody()->GetLinearVelocity().y));

				if (obj->getAttackBody() == nullptr)
				{
					b2BodyDef *bodydef = new b2BodyDef();

					float _P2M = 1.0f / 20.0f;

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

			}

			void PhysicsEngineActionHandler::midRangeAttackEnd(MovableGameObject *obj) {
				obj->getBody()->SetLinearVelocity(b2Vec2(obj->getBody()->GetLinearVelocity().x, obj->getBody()->GetLinearVelocity().y));

				if (obj->getAttackBody() != nullptr &&! obj->getBody()->GetWorld()->IsLocked())
				{
					delete static_cast<sdmg::model::Platform*>(obj->getAttackBody()->GetUserData());
					obj->getBody()->GetWorld()->DestroyBody(obj->getAttackBody());
					obj->setAttackBody(nullptr);
				}
			}

			void PhysicsEngineActionHandler::longRangeAttack(MovableGameObject *obj) {

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
				obj->setLives(obj->getLives() - 1);

				if (obj->getLives() == 0)
				{
					obj->die();
				}

				obj->setDirection(obj->getSpawnDirection());
				obj->setState(MovableGameObject::State::FALLING);
				obj->getBody()->SetTransform(b2Vec2(obj->getSpawnLocationX() / 20.0f, obj->getSpawnLocationY() / 20.0f), obj->getBody()->GetAngle());
				obj->getBody()->SetLinearVelocity(b2Vec2(0.0f, -1.0f));
			}
		}
	}
}