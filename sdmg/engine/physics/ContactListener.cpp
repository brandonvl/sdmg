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

				if (bodyA->GetType() == b2_dynamicBody && bodyA->GetPosition().y <  bodyB->GetPosition().y)
				{
					MovableGameObject *object = static_cast<MovableGameObject*>(bodyA->GetUserData());
					object->setIsJumping(false);
				}

				if (bodyB->GetType() == b2_dynamicBody && bodyB->GetPosition().y <  bodyA->GetPosition().y)
				{
 					MovableGameObject *object = static_cast<MovableGameObject*>(bodyB->GetUserData());
					object->setIsJumping(false);
				}

				if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_kinematicBody)
				{
					model::MovablePlatform *platform = static_cast<model::MovablePlatform*>(bodyB->GetUserData());
					if (platform->getDieOnImpact())
					{
						MovableGameObject *object = static_cast<MovableGameObject*>(bodyA->GetUserData());
						object->setState(MovableGameObject::State::RESPAWN);
					}
				}
				if (bodyA->GetType() == b2_kinematicBody && bodyB->GetType() == b2_dynamicBody)
				{
					model::MovablePlatform *platform = static_cast<model::MovablePlatform*>(bodyA->GetUserData());
					if (platform->getDieOnImpact())
					{
						MovableGameObject *object = static_cast<MovableGameObject*>(bodyB->GetUserData());
						object->setState(MovableGameObject::State::RESPAWN);
					}
				}

				/*
				std::cout << "I'm Hit";
				if (contact->GetFixtureA()->GetBody()->GetType() == b2_kinematicBody)
				{
				contact->GetFixtureB()->GetBody()->SetLinearVelocity(contact->GetFixtureA()->GetBody()->GetLinearVelocity());
				KinematicBody *kinematicBody = static_cast<KinematicBody*>(contact->GetFixtureA()->GetBody()->GetUserData()); // (*_kinecticBodies)[tmp];
				kinematicBody->addBody(contact->GetFixtureB()->GetBody(), contact->GetFixtureA()->GetBody()->GetLinearVelocity());
				}
				std::cout << contact->GetFixtureA() << " botst tegen " << contact->GetFixtureB() << std::endl;
				*/
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
				//  std::cout << "PreSolve" << std::endl;
			}

			void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse *impulse) {
				//  std::cout << "Postsolve" << std::endl;
			}
		}
	}
}