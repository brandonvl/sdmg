#include "ContactListener.h"
#include "engine\physics\KinematicBody.h"
#include "engine\MovableGameObject.h"
#include "model\MovablePlatform.h"
#include "model\Platform.h"
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
					bool resetWalk = false;
					if (bodyB->GetType() == b2_dynamicBody)
						resetWalk = true;
					else
					{
						model::Platform *platform = static_cast<model::Platform*>(bodyB->GetUserData());
						resetWalk = !platform->getIsAttack();
					}
					if (resetWalk)
					{
						MovableGameObject *object = static_cast<MovableGameObject*>(bodyA->GetUserData());
						object->setIsJumping(false);

						MovableGameObject::State state = object->getState();

						if (state == MovableGameObject::State::FALLING || state == MovableGameObject::State::JUMPING)
							object->setState(MovableGameObject::State::IDLE);
						else if (state == MovableGameObject::State::FALLINGLEFT || state == MovableGameObject::State::FALLINGRIGHT
							|| state == MovableGameObject::State::JUMPINGLEFT || state == MovableGameObject::State::JUMPINGRIGHT)
							object->setState(MovableGameObject::State::WALKING);
					}
				}
				else if (bodyB->GetType() == b2_dynamicBody && bodyB->GetPosition().y < bodyA->GetPosition().y)
				{
					bool resetWalk = false;
					if (bodyA->GetType() == b2_dynamicBody)
						resetWalk = true;
					else
					{
						model::Platform *platform = static_cast<model::Platform*>(bodyA->GetUserData());
						resetWalk = !platform->getIsAttack();
					}
					if (resetWalk)
					{
						MovableGameObject *object = static_cast<MovableGameObject*>(bodyB->GetUserData());
						object->setIsJumping(false);

						MovableGameObject::State state = object->getState();

						if (state == MovableGameObject::State::FALLING || state == MovableGameObject::State::JUMPING)
							object->setState(MovableGameObject::State::IDLE);
						else if (state == MovableGameObject::State::FALLINGLEFT || state == MovableGameObject::State::FALLINGRIGHT
							|| state == MovableGameObject::State::JUMPINGLEFT || state == MovableGameObject::State::JUMPINGRIGHT)
							object->setState(MovableGameObject::State::WALKING);
					}
				}
				// Springen terugzetten ---------------------------------------------------


				// In aaraking komen met een kinematic body -------------------------------
				if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_kinematicBody)
				{
					model::MovablePlatform *platform = static_cast<model::MovablePlatform*>(bodyB->GetUserData());

					if (platform->getDamageOnImpact() > 0)
					{
						MovableGameObject *player = static_cast<MovableGameObject*>(bodyA->GetUserData());
						if (bodyB != player->getShootBody())
						{
							player->hit(platform->getDamageOnImpact());
							platform->setMustBeDestroyed(platform->getDamageOnImpact() < 100);
						}
					}
				}
				else if (bodyA->GetType() == b2_kinematicBody && bodyB->GetType() == b2_dynamicBody)
				{
					model::MovablePlatform *platform = static_cast<model::MovablePlatform*>(bodyA->GetUserData());

					if (platform->getDamageOnImpact() > 0)
					{
						MovableGameObject *player = static_cast<MovableGameObject*>(bodyB->GetUserData());
						if (bodyA != player->getShootBody())
						{
							player->hit(platform->getDamageOnImpact());
							platform->setMustBeDestroyed(platform->getDamageOnImpact() < 100);
						}
					}
				}
				// In aaraking komen met een kinematic body -------------------------------

				// Iemand raak slaan ------------------------------------------------------
				if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_staticBody)
				{
					model::Platform *platform = static_cast<model::Platform*>( bodyB->GetUserData());
					if (platform->getIsAttack())
					{
						MovableGameObject *player = static_cast<MovableGameObject*>(bodyA->GetUserData());
						MovableGameObject::State state = player->getState();
						if (bodyB != player->getAttackBody())
						{
							if (state != MovableGameObject::State::KNOCKBACKLEFT && state != MovableGameObject::State::KNOCKBACKRIGHT
								&& state != (MovableGameObject::State::IDLE | MovableGameObject::State::FORWARD_ROLL)
								&& state != (MovableGameObject::State::WALKING | MovableGameObject::State::FORWARD_ROLL))
							{
								float x = player->getX();
								float x2 = platform->getX();

								if (player->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::BLOCKING))
								{
									player->setBP(player->getBP() - 10);
								}
								else
								{
									if (player->getX() > platform->getX())
										player->setState(MovableGameObject::State::KNOCKBACKRIGHT);
									else
										player->setState(MovableGameObject::State::KNOCKBACKLEFT);
									player->hit(10);
								}
							}
						}
					}
				}
				/*
				else if (bodyA->GetType() == b2_staticBody && bodyB->GetType() == b2_dynamicBody)
				{
					model::Platform *platform = static_cast<model::Platform*>(bodyA->GetUserData());
					if (platform->getIsAttack())
					{
						MovableGameObject *player = static_cast<MovableGameObject*>(bodyB->GetUserData());
						MovableGameObject::State state = player->getState();

						if (state != MovableGameObject::State::KNOCKBACKLEFT && state != MovableGameObject::State::KNOCKBACKRIGHT)
						{
							if (player->getX() > platform->getX())
								player->setState(MovableGameObject::State::KNOCKBACKRIGHT);
							else
								player->setState(MovableGameObject::State::KNOCKBACKLEFT);
							player->setHP(player->getHP() - 10);
						}
					}
				}
				*/
				// Iemand raak slaan ------------------------------------------------------


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

				contact->SetFriction(0.0f);
				b2Body *bodyA = contact->GetFixtureA()->GetBody(), *bodyB = contact->GetFixtureB()->GetBody();

				// Door een character heenrollen ------------------------------------------
				if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_dynamicBody)
				{
					MovableGameObject *player = static_cast<MovableGameObject*>(bodyA->GetUserData());
					MovableGameObject *player2 = static_cast<MovableGameObject*>(bodyB->GetUserData());
					if (player->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::FORWARD_ROLL)
						|| player->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::FORWARD_ROLL)
						|| player2->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::FORWARD_ROLL)
						|| player2->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::FORWARD_ROLL))
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
						if (bodyB->GetType() == b2_kinematicBody)
						{
							model::MovablePlatform *platform = static_cast<model::MovablePlatform*>(bodyB->GetUserData());
							if (platform->getDamageOnImpact() <= 0)
								contact->SetEnabled(false);
						}
						else
							contact->SetEnabled(false);
					}
				}
				else if (bodyB->GetType() == b2_dynamicBody && (bodyA->GetType() == b2_kinematicBody || bodyA->GetType() == b2_staticBody))
				{
					MovableGameObject *player = static_cast<MovableGameObject*>(bodyB->GetUserData());
					float yPlayer = player->getY() + (player->getHeight() * ( 1 / 20));

					if (yPlayer + 5.0f > bodyA->GetPosition().y)
					{
						if (bodyA->GetType() == b2_kinematicBody)
						{
							model::MovablePlatform *platform = static_cast<model::MovablePlatform*>(bodyA->GetUserData());
							if (platform->getDamageOnImpact() <= 0)
								contact->SetEnabled(false);
						}
						else
							contact->SetEnabled(false);
					}
				}
				// Onderdoor een platform heensprigen -------------------------------------
			}

			void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse *impulse) {
				contact->SetFriction(0.0f);
				b2Body *bodyA = contact->GetFixtureA()->GetBody(), *bodyB = contact->GetFixtureB()->GetBody();

				/*
				// Iemand raak slaan ------------------------------------------------------
				if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_dynamicBody)
				{
					MovableGameObject *player1 = static_cast<MovableGameObject*>(bodyA->GetUserData()),
						*player2 = static_cast<MovableGameObject*>(bodyB->GetUserData());
					MovableGameObject::State state1 = player1->getState(), state2 = player2->getState();

					if (state1 != MovableGameObject::State::KNOCKBACKLEFT && state1 != MovableGameObject::State::KNOCKBACKRIGHT
						&& state2 != MovableGameObject::State::KNOCKBACKLEFT && state2 != MovableGameObject::State::KNOCKBACKRIGHT)
					{
						if (player1->getState() == MovableGameObject::State::MIDRANGEATTACK)
						{
							if (player1->getX() > player2->getX())
								player2->setState(MovableGameObject::State::KNOCKBACKLEFT);
							else
								player2->setState(MovableGameObject::State::KNOCKBACKRIGHT);
							player2->setHP(player2->getHP() - 10);
						}
						else if (player2->getState() == MovableGameObject::State::MIDRANGEATTACK)
						{
							if (player2->getX() > player1->getX())
								player1->setState(MovableGameObject::State::KNOCKBACKLEFT);
							else
								player1->setState(MovableGameObject::State::KNOCKBACKRIGHT);
							player1->setHP(player1->getHP() - 10);
						}
					}
				}
				// Iemand raak slaan ------------------------------------------------------
				*/
			}
		}
	}
}