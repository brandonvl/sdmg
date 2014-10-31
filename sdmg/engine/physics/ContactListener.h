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
			};
		}
	}
}
