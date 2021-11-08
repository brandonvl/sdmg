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
#include <functional>

namespace sdmg {
	namespace engine {
		class GameBase;
		class GameObject;
		class MovableGameObject;

		class World {
		public:
			World(GameBase &game) : _game(&game) {};
			virtual ~World();
			const std::vector<GameObject*> &getGameObjects();
			const std::vector<GameObject*> &getPlatforms();
			const std::vector<MovableGameObject*> &getPlayers();
			GameObject* getGameObjectById(uint32 id);
			b2World* getPhysicsWorld();
			void addGameObject(GameObject *gameObject);
			void addDeadCallBack(std::function<void(GameObject *gameObject)> &callBack);
			void addPlayer(MovableGameObject *player);
			void addPlatform(GameObject *platform);
			void onDead(GameObject* gameObject);
			bool isGameOver();
			void addAlive(GameObject *gameObject);
			const std::vector<GameObject*> &getDeadList();
			void clearDeadList();
			const std::vector<GameObject*> &getAliveList();
			void clearAliveList();
			void clearWorld();
			void resetWorld();
			void removePlatform(GameObject *platform);
			void destroyShootBodies();
			bool platformExists(GameObject *platform);
		private:
			std::vector<GameObject*> _gameObjects, _deadList, _aliveList, _platforms;
			std::vector<std::function<void(GameObject *gameObject)>> _deadCallBacks;
			std::vector<MovableGameObject*> _players;
			GameBase *_game;
			b2World *_physicsWorld;
		};
	}
}