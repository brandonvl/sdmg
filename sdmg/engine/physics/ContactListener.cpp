#include "ContactListener.h"
#include "engine\physics\KinematicBody.h"
#include "engine\MovableGameObject.h"
#include "model\MovablePlatform.h"
#include "PhysicsEngine.h"
#include <iostream>

namespace sdmg {
	namespace engine {
		namespace physics {
			ContactListener::ContactListener() { }
			ContactListener::~ContactListener() { }

			void ContactListener::BeginContact(b2Contact* contact) {
				contact->SetFriction(0.0f);

				b2Body *bodyA = contact->GetFixtureA()->GetBody(), *bodyB = contact->GetFixtureB()->GetBody();


				// Springen terugzetten ---------------------------------------------------
				if (bodyA->GetType() == b2_dynamicBody && bodyA->GetPosition().y <  bodyB->GetPosition().y)
				{
					MovableGameObject *object = static_cast<MovableGameObject*>(bodyA->GetUserData());
					object->setIsJumping(false);

					if (object->getState() == MovableGameObject::State::FALLING)
						object->setState(MovableGameObject::State::IDLE);
					else if (object->getState() == MovableGameObject::State::FALLINGLEFT || object->getState() == MovableGameObject::State::FALLINGRIGHT)
						object->setState(MovableGameObject::State::WALKING);
				}
				else if (bodyB->GetType() == b2_dynamicBody && bodyB->GetPosition().y <  bodyA->GetPosition().y)
				{
 					MovableGameObject *object = static_cast<MovableGameObject*>(bodyB->GetUserData());
					object->setIsJumping(false);

					if (object->getState() == MovableGameObject::State::FALLING)
						object->setState(MovableGameObject::State::IDLE);
					else if (object->getState() == MovableGameObject::State::FALLINGLEFT || object->getState() == MovableGameObject::State::FALLINGRIGHT)
						object->setState(MovableGameObject::State::WALKING);
				}
				// Springen terugzetten ---------------------------------------------------


				// In aaraking komen met een kinematic body -------------------------------
				if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_kinematicBody)
				{
					model::MovablePlatform *platform = static_cast<model::MovablePlatform*>(bodyB->GetUserData());

					if (platform->getDieOnImpact())
					{
						MovableGameObject *player = static_cast<MovableGameObject*>(bodyA->GetUserData());
						player->setState(MovableGameObject::State::RESPAWN);
					}
				}
				else if (bodyA->GetType() == b2_kinematicBody && bodyB->GetType() == b2_dynamicBody)
				{
					model::MovablePlatform *platform = static_cast<model::MovablePlatform*>(bodyA->GetUserData());

					if (platform->getDieOnImpact())
					{
						MovableGameObject *player = static_cast<MovableGameObject*>(bodyB->GetUserData());
						player->setState(MovableGameObject::State::RESPAWN);
					}
				}
				// In aaraking komen met een kinematic body -------------------------------

			}

			void ContactListener::EndContact(b2Contact* contact) {
				/*
				if (contact->GetFixtureA()->GetBody()->GetType() == b2_kinematicBody)
				{
				KinematicBody *kinematicBody = static_cast<KinematicBody*>(contact->GetFixtureA()->GetBody()->GetUserData()); // (*_kinecticBodies)[tmp];
				kinematicBody->removeBody(contact->GetFixtureB()->GetBody());
				}
				*/
			}

			void ContactListener::PreSolve(b2Contact* contact, const b2Manifold *oldManifold) {

				b2Body *bodyA = contact->GetFixtureA()->GetBody(), *bodyB = contact->GetFixtureB()->GetBody();
				
				if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_dynamicBody)
				{

					MovableGameObject *player = static_cast<MovableGameObject*>(bodyA->GetUserData());
					MovableGameObject *player2 = static_cast<MovableGameObject*>(bodyB->GetUserData());

					if (player->getState() == MovableGameObject::State::FORWARD_ROLL || player2->getState() == MovableGameObject::State::FORWARD_ROLL)
					{
						contact->SetEnabled(false);
					}
				}
				


				if (bodyA->GetType() == b2_dynamicBody && (bodyB->GetType() == b2_kinematicBody || bodyB->GetType() == b2_staticBody))
				{
					MovableGameObject *player = static_cast<MovableGameObject*>(bodyA->GetUserData());
					float yPlayer = player->getY() + (player->getHeight() * (1 / 20));

					if (yPlayer + 5.0f > bodyB->GetPosition().y)
					{
						contact->SetEnabled(false);
					}
				}
				else if (bodyB->GetType() == b2_dynamicBody && (bodyA->GetType() == b2_kinematicBody || bodyA->GetType() == b2_staticBody))
				{
					MovableGameObject *player = static_cast<MovableGameObject*>(bodyB->GetUserData());
					float yPlayer = player->getY() + (player->getHeight() * ( 1 / 20));

					if (yPlayer + 5.0f > bodyA->GetPosition().y)
					{
						contact->SetEnabled(false);
					}
				}

			}

			void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse *impulse) {

			}
		}
	}
}