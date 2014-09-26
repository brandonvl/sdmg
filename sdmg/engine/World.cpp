//
//
//  @ Project : SDMG
//  @ File Name : World.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "World.h"
#include "GameObject.h"

namespace sdmg {
	namespace engine {
		std::map<int,GameObject*>* World::getGameObjects() {
			return _gameObjects;
		}
		
		GameObject* World::getGameObjectById(int id) {
			// TODO: Find gameobject by ID
			return nullptr;
		}
		
		b2World* World::getPhysicsWorld() {
			return _physicsWorld;
		}
		
	}
}
