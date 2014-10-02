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
				std::cout << "Naar links gaan" << std::endl;
				obj->getBody()->SetLinearVelocity(b2Vec2(obj->getHorizontalSpeed() * -1, obj->getBody()->GetLinearVelocity().y));

			}

			void PhysicsEngineActionHandler::moveRight(MovableGameObject *obj) {
				obj->getBody()->SetLinearVelocity(b2Vec2(obj->getHorizontalSpeed(), obj->getBody()->GetLinearVelocity().y));
			}

			void PhysicsEngineActionHandler::idle(MovableGameObject *obj) {
				obj->getBody()->SetLinearVelocity(b2Vec2(0.0f, obj->getBody()->GetLinearVelocity().y));
			}

			void PhysicsEngineActionHandler::jump(MovableGameObject *obj) {
				b2Vec2 jumpImpulse(0.0f, -(obj->getBody()->GetMass() * obj->getVerticalSpeed()));
				//_body->ApplyLinearImpulse(jumpImpulse, _body->GetWorldCenter(), true);

				obj->getBody()->SetLinearVelocity(jumpImpulse);
			}

			void PhysicsEngineActionHandler::shortAttack(MovableGameObject *obj) {

			}

			void PhysicsEngineActionHandler::middleAttack(MovableGameObject *obj) {

			}

			void PhysicsEngineActionHandler::longAttack(MovableGameObject *obj) {

			}
		}
	}
}