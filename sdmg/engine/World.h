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
		class MovableGameObject;

		class World {
		public:
			virtual ~World();
			const std::vector<GameObject*> &getGameObjects();
			const std::vector<GameObject*> &getPlatforms();
			const std::vector<MovableGameObject*> &getPlayers();
			GameObject* getGameObjectById(uint32 id);
			b2World* getPhysicsWorld();
			void addGameObject(GameObject *gameObject);
			void addPlayer(MovableGameObject *player);
			void addPlatform(GameObject *platform);
			void onDead(GameObject* gameObject);
			bool isGameOver();
			void addAliveToDead();
			const std::vector<GameObject*> &getDeadList();
			const std::vector<GameObject*> &getAliveList();
			void clearWorld();
			void resetWorld();
		private:
			std::vector<GameObject*> _gameObjects, _deadList, _aliveList, _platforms;
			std::vector<MovableGameObject*> _players;

			b2World *_physicsWorld;
		};
	}
}