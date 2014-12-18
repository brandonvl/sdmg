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
#include "MovableGameObject.h"
#include "GameBase.h"

#include <algorithm>

namespace sdmg {
	namespace engine {
		const std::vector<GameObject*> &World::getGameObjects() {
			return _gameObjects;
		}

		World::~World() {
			clearWorld();
		}
		
		GameObject* World::getGameObjectById(uint32 id) {
			if (_gameObjects.size() > id)
				return _gameObjects[id];
			return nullptr;
		}
		
		b2World* World::getPhysicsWorld() {
			return _physicsWorld;
		}

		void World::destroyShootBodies() {

			for (auto &obj : _players) {
				obj->destroyShootBody(*_game->getEngine());
			}

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

		void World::addAlive(GameObject *gameObject)
		{
			_aliveList.push_back(gameObject);
		}

		const std::vector<GameObject*> &World::getDeadList() {
			return _deadList;
		}

		void World::clearDeadList()
		{
			_deadList.clear();
		}

		const std::vector<GameObject*> &World::getAliveList() {
			return _aliveList;
		}

		void World::clearAliveList()
		{
			_aliveList.clear();
		}

		const std::vector<GameObject*> &World::getPlatforms() {
			return _platforms;
		}

		const std::vector<MovableGameObject*> &World::getPlayers() {
			return _players;
		}

		void World::clearWorld()
		{
			for (auto it : _gameObjects)
			{
				delete it;
			}
			_deadList.clear();
			_aliveList.clear();
			_gameObjects.clear();
			_players.clear();
			_platforms.clear();
		}

		void World::resetWorld()
		{
			_aliveList.clear();
			_aliveList = _deadList;
			_deadList.clear();
		}

		void World::addPlatform(GameObject *platform) {
			_platforms.push_back(platform);
			addGameObject(platform);
		}

		void World::addPlayer(MovableGameObject *player) {
			_players.push_back(player);
			addGameObject(player);
		}

		void World::removePlatform(GameObject *platform)
		{
			// Toch handig dat we precies de presentatie hiervoor hebben gehad :D
			auto p = remove(_platforms.begin(), _platforms.end(), platform);
			_platforms.erase(p, _platforms.end());

			auto g = remove(_gameObjects.begin(), _gameObjects.end(), platform);
			_gameObjects.erase(g, _gameObjects.end());
		}
	}
}
