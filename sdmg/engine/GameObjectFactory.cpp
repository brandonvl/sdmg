//
//
//  @ Project : SDMG
//  @ File Name : GameObjectFactory.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#include "GameObjectFactory.h"
#include <string>
#include "GameObject.h"

namespace sdmg {
	namespace engine {
		GameObjectFactory *GameObjectFactory::_instance = 0;

		GameObjectFactory::GameObjectFactory() {

		}

		GameObjectFactory::~GameObjectFactory() {

		}

		void GameObjectFactory::create(std::string key) {
		
		}
				
		void GameObjectFactory::giveIdentifier(GameObject *gameObject) {
		
		}

		GameObjectFactory* GameObjectFactory::getInstance() {
			if (!_instance) _instance = new GameObjectFactory();			
			return _instance;
		}
		
	}
}
