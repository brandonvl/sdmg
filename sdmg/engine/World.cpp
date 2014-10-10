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
		const std::vector<GameObject*> &World::getGameObjects() {
			return _gameObjects;
		}
		
		GameObject* World::getGameObjectById(uint32 id) {
			if (_gameObjects.size() > id)
				return _gameObjects[id];
			return nullptr;
		}
		
		b2World* World::getPhysicsWorld() {
			return _physicsWorld;
		}
		
		void World::addGameObject(GameObject *gameObject) {
			_gameObjects.push_back(gameObject);
			gameObject->bindWorld(this, _gameObjects.size() - 1);

			if (gameObject->getFlags() == GameObject::Flags::CANDIE) {
				_aliveList.push_back(gameObject);
			}
		}

		void World::onDead(GameObject* gameObject) {
			_deadList.push_back(gameObject);

			for (int i = 0; i < _aliveList.size(); i++)
			{
				if (_aliveList[i] == gameObject)
				{
					_aliveList.erase(_aliveList.begin() + i);
					break;
				}
			}
		}

		bool World::isGameOver() {
			return _aliveList.size() <= 1;
		}

		const std::vector<GameObject*> &World::getDeadList() {
			return _deadList;
		}

		const std::vector<GameObject*> &World::getAliveList() {
			return _aliveList;
		}

		void World::clearWorld()
		{
			_deadList.clear();
			_aliveList.clear();
			_gameObjects.clear();
		}
	}
}
