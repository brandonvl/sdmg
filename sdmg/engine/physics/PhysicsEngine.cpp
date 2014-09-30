//
//
//  @ Project : SDMG
//  @ File Name : PhysicsEngine.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "PhysicsEngine.h"
#include "engine\Engine.h"
#include "engine\GameBase.h"
#include "engine\World.h"
#include "Box2D\Box2D.h"

namespace sdmg {
	namespace engine {
		namespace physics {
			void PhysicsEngine::update() {
				if (_enabled)
					_world->Step(1 / 60, 8, 3);
			}
			
			void PhysicsEngine::pause() {
				_enabled = false;
			}
			
			void PhysicsEngine::resume() {
				_enabled = true;
			}

			void PhysicsEngine::createWorld(const float leftGravity, const float downGravity)
			{
				_world = new b2World(b2Vec2(leftGravity, downGravity));
				//  _world->SetGravity(b2Vec2(leftGravity, downGravity));
			}

			void CreateBody()
			{

			}
		}
	}
}
