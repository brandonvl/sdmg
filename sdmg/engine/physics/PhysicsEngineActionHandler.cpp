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
			}

			void PhysicsEngineActionHandler::moveRight(MovableGameObject *obj) {
				std::cout << "Naar rechts gaan" << std::endl;
			}

			void PhysicsEngineActionHandler::idle(MovableGameObject *obj) {
				std::cout << "Nergens heen gaan" << std::endl;
			}

			void PhysicsEngineActionHandler::jump(MovableGameObject *obj) {
				std::cout << "Springen met die banaan" << std::endl;
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