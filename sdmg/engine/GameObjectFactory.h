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
			static GameObjectFactory* getInstance();
		private:
			GameObjectFactory();
			virtual ~GameObjectFactory();
			static GameObjectFactory *_instance;
			int _gameObjectIdentifier;
			std::map<std::string,GameObject*> *_gameObjectHolder;
			void giveIdentifier(GameObject *gameObject);
		};
	}
}