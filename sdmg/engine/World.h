//
//
//  @ Project : SDMG
//  @ File Name : World.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include <vector>
#include <Box2D\Box2D.h>

namespace sdmg {
	namespace engine {
		class GameObject;

		class World {
		public:
			const std::vector<GameObject*> &getGameObjects();
			GameObject* getGameObjectById(uint32 id);
			b2World* getPhysicsWorld();
			void addGameObject(GameObject *gameObject);
			void onDead(GameObject* gameObject);
			bool isGameOver();
			void addAliveToDead();
			const std::vector<GameObject*> &getDeadList();
			const std::vector<GameObject*> &getAliveList();
			void clearWorld();
		private:
			std::vector<GameObject*> _gameObjects, _deadList, _aliveList;

			b2World *_physicsWorld;
		};
	}
}