#include "ContactListener.h"
#include "engine\physics\KinematicBody.h"
#include "engine\MovableGameObject.h"
#include "model\MovablePlatform.h"
#include "engine\MovableGameObject.h"
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
				if (bodyA->GetType() == b2_dynamicBody && bodyA->GetPosition().y < bodyB->GetPosition().y)
					resetJump(bodyA, bodyB);
				else if (bodyB->GetType() == b2_dynamicBody && bodyB->GetPosition().y < bodyA->GetPosition().y)
					resetJump(bodyB, bodyA);
				// Springen terugzetten ---------------------------------------------------

				// Een dynamic body in aanraking komen met een kinematic body -------------------------------
				if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_kinematicBody)
					ContactDynamicBodyWithKinematicBody(bodyA, bodyB, contact);
				else if (bodyA->GetType() == b2_kinematicBody && bodyB->GetType() == b2_dynamicBody)
					ContactDynamicBodyWithKinematicBody(bodyB, bodyA, contact);
				// Een dynamic body in aanraking komen met een kinematic body -------------------------------

				// Een kinematic body in aanraking komen met een kinematic body -------------------------------
				if (bodyA->GetType() == b2_kinematicBody && bodyB->GetType() == b2_kinematicBody)
					ContactKinematicBodyWithKinematicBody(bodyA, bodyB);
				else if (bodyB->GetType() == b2_kinematicBody && bodyA->GetType() == b2_kinematicBody)
					ContactKinematicBodyWithKinematicBody(bodyB, bodyA);
				// Een kinematic body in aanraking komen met een kinematic body -------------------------------

				// Een kinematic body in aanraking komen met een static body -------------------------------
				if (bodyA->GetType() == b2_kinematicBody && bodyB->GetType() == b2_staticBody)
					ContactKinematicBodyWithStaticBody(bodyA, bodyB);
				else if (bodyB->GetType() == b2_kinematicBody && bodyA->GetType() == b2_staticBody)
					ContactKinematicBodyWithStaticBody(bodyB, bodyA);
				// Een kinematic body in aanraking komen met een static body -------------------------------

				// Iemand raak slaan ------------------------------------------------------
				if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_staticBody)
					gotHit(bodyA, bodyB);
				else if (bodyA->GetType() == b2_staticBody && bodyB->GetType() == b2_dynamicBody)
					gotHit(bodyB, bodyA);
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
					if (player->getRolling() || player2->getRolling())
						contact->SetEnabled(false);
				}
				// Door een character heenrollen ------------------------------------------

				// Door een kogel heenrollen ------------------------------------------
				if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_kinematicBody)
					ContactKinematicBodyWithDynamicBody(bodyB, bodyA, contact);
				else if (bodyA->GetType() == b2_kinematicBody && bodyB->GetType() == b2_dynamicBody)
					ContactKinematicBodyWithDynamicBody(bodyA, bodyB, contact);
				// Door een kogel heenrollen ------------------------------------------

				// Onderdoor een platform heenspringen -------------------------------------
				if (bodyA->GetType() == b2_dynamicBody && (bodyB->GetType() == b2_kinematicBody || bodyB->GetType() == b2_staticBody))
					jumpThroughPlatform(bodyA, bodyB, contact);
				else if (bodyB->GetType() == b2_dynamicBody && (bodyA->GetType() == b2_kinematicBody || bodyA->GetType() == b2_staticBody))
					jumpThroughPlatform(bodyB, bodyA, contact);
				// Onderdoor een platform heenspringen -------------------------------------
			}

			void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse *impulse) {
				/*
				contact->SetFriction(0.0f);
				b2Body *bodyA = contact->GetFixtureA()->GetBody(), *bodyB = contact->GetFixtureB()->GetBody();

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

			void ContactListener::resetJump(b2Body *upper, b2Body *lower)
			{
				model::Platform *platform = nullptr;
				bool resetWalk = false;
				if (lower->GetType() == b2_dynamicBody)
					resetWalk = true;
				else
				{
					model::Platform *platform = static_cast<model::Platform*>(lower->GetUserData());
					// resetWalk = !platform->getIsAttack();
					resetWalk = platform->getDamageOnImpact() == 0;
				}
				if (resetWalk)
				{
					MovableGameObject *object = static_cast<MovableGameObject*>(upper->GetUserData());

					if (lower != object->getAttackBody() && lower != object->getShootBody())
					{
						object->setIsJumping(false);

						MovableGameObject::State state = object->getState();

						if (state == MovableGameObject::State::FALLING || state == MovableGameObject::State::JUMPING)
							object->setState(MovableGameObject::State::IDLE);
						else if (state == MovableGameObject::State::FALLINGLEFT || state == MovableGameObject::State::FALLINGRIGHT
							|| state == MovableGameObject::State::JUMPINGLEFT || state == MovableGameObject::State::JUMPINGRIGHT)
							object->setState(MovableGameObject::State::WALKING);
					}
				}
			}

			void ContactListener::ContactDynamicBodyWithKinematicBody(b2Body *dynamic, b2Body *kinematic, b2Contact *contact)
			{
				model::MovablePlatform *platform = static_cast<model::MovablePlatform*>(kinematic->GetUserData());

				if (platform->getDamageOnImpact() > 0)
				{
					MovableGameObject *player = static_cast<MovableGameObject*>(dynamic->GetUserData());
					if (player->getRolling())
						contact->SetEnabled(false);
					else if (kinematic != player->getShootBody())
					{
						MovableGameObject::State state = player->getState();
						if (state != MovableGameObject::State::KNEELING && state != MovableGameObject::State::KNOCKBACKLEFT && state != MovableGameObject::State::KNOCKBACKRIGHT)
						{
							if (player->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::BLOCKING))
								player->addPP(-platform->getDamageOnImpact());
							else
							{
								if (player->getX() > platform->getX())
									player->setState(MovableGameObject::State::KNOCKBACKRIGHT);
								else
									player->setState(MovableGameObject::State::KNOCKBACKLEFT);
								player->hit(platform->getDamageOnImpact());
							}
						}
						platform->setMustBeDestroyed(platform->getDamageOnImpact() < 100);
					}
				}
			}
			
			void ContactListener::gotHit(b2Body *playerBody, b2Body *weapon)
			{
				model::Platform *platform = static_cast<model::Platform*>(weapon->GetUserData());
				if (platform->getDamageOnImpact() > 0)
				{
					MovableGameObject *player = static_cast<MovableGameObject*>(playerBody->GetUserData());
					MovableGameObject::State state = player->getState();
					if (weapon != player->getAttackBody())
					{
						if (state != MovableGameObject::State::KNEELING && state != MovableGameObject::State::KNOCKBACKLEFT && state != MovableGameObject::State::KNOCKBACKRIGHT && !player->getRolling())
						{
							if (player->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::BLOCKING))
								player->addPP(-platform->getDamageOnImpact());
							else
							{
								if (player->getX() > platform->getX())
									player->setState(MovableGameObject::State::KNOCKBACKRIGHT);
								else
									player->setState(MovableGameObject::State::KNOCKBACKLEFT);
								player->hit(platform->getDamageOnImpact());
							}
						}
					}
				}
			}

			void ContactListener::jumpThroughPlatform(b2Body *playerBody, b2Body *platformBody, b2Contact *contact)
			{
				MovableGameObject *player = static_cast<MovableGameObject*>(playerBody->GetUserData());
				float yPlayer = player->getY() + (player->getHeight() * (1 / 20));

				if (yPlayer + 5.0f > platformBody->GetPosition().y)
				{
					bool enabled = true;
					model::MovablePlatform *platform = static_cast<model::MovablePlatform*>(platformBody->GetUserData());
					if ((player->getIsJumping() || player->getFalling()) && player->getBody()->GetLinearVelocity().y >= -8 && platform->getDamageOnImpact() == 0 && platformBody != player->getAttackBody())
						player->setState(MovableGameObject::State::IDLE);

					if (platformBody->GetType() == b2_kinematicBody)
					{
						if (platform->getDamageOnImpact() <= 0)
							contact->SetEnabled(false);
					}
					else
					{
						model::Platform *platformStatic = static_cast<model::Platform*>(platformBody->GetUserData());
						if(platformStatic->getCanMoveThroughIt())
							contact->SetEnabled(false);
					}
				}
			}

			void ContactListener::ContactKinematicBodyWithKinematicBody(b2Body *kinematicBody1, b2Body *kinematicBody2)
			{
				model::MovablePlatform *kinematic1 = static_cast<model::MovablePlatform*>(kinematicBody1->GetUserData()),
					*kinematic2 = static_cast<model::MovablePlatform*>(kinematicBody2->GetUserData());
				kinematic1->setMustBeDestroyed(kinematic1->getDamageOnImpact() > 0);
				kinematic2->setMustBeDestroyed(kinematic2->getDamageOnImpact() > 0);
			}

			void ContactListener::ContactKinematicBodyWithStaticBody(b2Body *kinematicBody, b2Body *staticBody)
			{
				model::MovablePlatform *kinematic = static_cast<model::MovablePlatform*>(kinematicBody->GetUserData());
				//  model::Platform	*platform = static_cast<model::Platform*>(staticBody->GetUserData());
				kinematic->setMustBeDestroyed(kinematic->getDamageOnImpact() > 0);
			}

			void ContactListener::ContactKinematicBodyWithDynamicBody(b2Body *kinematic, b2Body *dynamic, b2Contact *contact)
			{
				MovableGameObject *player = static_cast<MovableGameObject*>(dynamic->GetUserData());
				model::MovablePlatform *platform = static_cast<model::MovablePlatform*>(kinematic->GetUserData());
				if (player->getRolling())
					contact->SetEnabled(false);
			}
		}
	}
}