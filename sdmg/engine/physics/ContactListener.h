#pragma once
#include "Box2D\Box2D.h"

namespace sdmg {
	namespace engine {
		namespace physics {
			class PhysicsEngine;

			class ContactListener : public b2ContactListener
			{
			public:
				ContactListener();
				virtual ~ContactListener();

				virtual void BeginContact(b2Contact* contact);
				virtual void EndContact(b2Contact* contact);
				virtual void PreSolve(b2Contact* contact, const b2Manifold *oldManifold);
				virtual void PostSolve(b2Contact* contact, const b2ContactImpulse *impulse);
			private:
				void resetJump(b2Body *upper, b2Body *lower);
				void ContactDynamicBodyWithKinematicBody(b2Body *dynamic, b2Body *kinematic, b2Contact *contact);
				void ContactKinematicBodyWithDynamicBody(b2Body *kinematic, b2Body *dynamic, b2Contact *contact);
				void ContactKinematicBodyWithKinematicBody(b2Body *kinematicBody1, b2Body *kinematicBody2);
				void ContactKinematicBodyWithStaticBody(b2Body *kinematicBody, b2Body *staticBody);
				void gotHit(b2Body *playerBody, b2Body *weapon);

				void jumpThroughPlatform(b2Body *playerBody, b2Body *platformBody, b2Contact *contact);
			};
		}
	}
}
