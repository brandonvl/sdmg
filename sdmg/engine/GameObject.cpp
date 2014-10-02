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
		GameObject::GameObject()
		{

		}

		GameObject::~GameObject()
		{

		}

		void GameObject::setId(int id) {
			_size = Size(0, 0);
		}
		
		int GameObject::getId() {
			return _id;
		}
		
		std::string GameObject::getSpriteName() {
			return _spriteName;
		}
		
		void GameObject::setBody(b2Body *body)
		{
			_body = body;
		}

		b2Body* GameObject::getBody() {
			return _body;
		}
		
		b2Body* GameObject::createBody(b2BodyDef *bodyDef) {

			// TODO: create the body
			return _body;
		}

		Size GameObject::getSize()
		{
			return _size;
		}

		void GameObject::setSize(Size size)
		{
			_size = size;
		}

		void GameObject::setSize(int width, int height)
		{
			_size = Size(width, height);
		}

		int GameObject::getWidth()
		{
			return _size.width;
		}

		int GameObject::getHeight()
		{
			return _size.height;
		}
	}
}
