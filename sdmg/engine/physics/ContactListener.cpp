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

				// Door een character heenrollen ------------------------------------------
				if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_dynamicBody)
				{
					MovableGameObject *player = static_cast<MovableGameObject*>(bodyA->GetUserData());
					MovableGameObject *player2 = static_cast<MovableGameObject*>(bodyB->GetUserData());
					if (player->getState() == MovableGameObject::State::FORWARD_ROLL || player2->getState() == MovableGameObject::State::FORWARD_ROLL)
						contact->SetEnabled(false);
				}
				// Door een character heenrollen ------------------------------------------
				
				// Onderdoor een platform heensprigen -------------------------------------
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
				// Onderdoor een platform heensprigen -------------------------------------


			}

			void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse *impulse) {

				b2Body *bodyA = contact->GetFixtureA()->GetBody(), *bodyB = contact->GetFixtureB()->GetBody();

				// Iemand raak slaan ------------------------------------------------------
				if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_dynamicBody)
				{
					MovableGameObject *player1 = static_cast<MovableGameObject*>(bodyA->GetUserData()),
						*player2 = static_cast<MovableGameObject*>(bodyB->GetUserData());

					if (player1->getState() == MovableGameObject::State::MIDRANGEATTACK && (player2->getState() != MovableGameObject::State::KNOCKBACKLEFT && player2->getState() != MovableGameObject::State::KNOCKBACKRIGHT))
					{
						if (player1->getX() > player2->getX())
							player2->setState(MovableGameObject::State::KNOCKBACKLEFT);
						else
							player2->setState(MovableGameObject::State::KNOCKBACKRIGHT);
						player2->setHP(player2->getHP() - 10);
					}
					if (player2->getState() == MovableGameObject::State::MIDRANGEATTACK && (player1->getState() != MovableGameObject::State::KNOCKBACKLEFT && player1->getState() != MovableGameObject::State::KNOCKBACKRIGHT))
					{
						if (player2->getX() > player2->getX())
							player1->setState(MovableGameObject::State::KNOCKBACKLEFT);
						else
							player1->setState(MovableGameObject::State::KNOCKBACKRIGHT);
						player1->setHP(player1->getHP() - 10);
					}
				}
				// Iemand raak slaan ------------------------------------------------------
			}
		}
	}
}