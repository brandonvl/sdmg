//
//
//  @ Project : SDMG
//  @ File Name : GameObject.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "GameObject.h"

namespace sdmg {
	namespace engine {
		void GameObject::setId(int id) {
		
		}
		
		int GameObject::getId() {
			return _id;
		}
		
		std::string GameObject::getSpriteName() {
			return _spriteName;
		}
		
		b2Body* GameObject::getBody() {
			return _body;
		}
		
		b2Body* GameObject::createBody(b2BodyDef *bodyDef) {

			// TODO: create the body
			return _body;
		}
	}
}
