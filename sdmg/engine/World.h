//
//
//  @ Project : SDMG
//  @ File Name : World.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include <map>
#include <Box2D\Box2D.h>

namespace sdmg {
	namespace engine {
		class GameObject;

		class World {
		public:
			std::map<int,GameObject*>* getGameObjects();
			GameObject* getGameObjectById(int id);
			b2World* getPhysicsWorld();
		private:
			std::map<int,GameObject*> *_gameObjects;
			b2World *_physicsWorld;
		};
	}
}