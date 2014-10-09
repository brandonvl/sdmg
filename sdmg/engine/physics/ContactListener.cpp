#include "ContactListener.h"
#include "engine\physics\KinematicBody.h"
#include <iostream>

namespace sdmg {
	namespace engine {
		namespace physics {
			ContactListener::ContactListener()
			{
			}


			ContactListener::~ContactListener()
			{
			}

			void ContactListener::BeginContact(b2Contact* contact) {
				contact->SetFriction(0.0f);
				
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