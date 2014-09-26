//
//
//  @ Project : SDMG
//  @ File Name : GameObjectFactory.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include <map>

namespace sdmg {
	namespace engine {
		class GameObject;

		class GameObjectFactory {
		public:
			void create(std::string key);
			static GameObjectFactory& getInstance() {
				static GameObjectFactory instance;
				return instance;
			}
		private:
			int _gameObjectIdentifier;
			std::map<std::string,GameObject*> *_gameObjectHolder;
			void giveIdentifier(GameObject *gameObject);
			GameObjectFactory();
		};
	}
}