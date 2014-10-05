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

		void GameObject::setSize(float width, float height)
		{
			_size = Size(width, height);
		}

		void GameObject::setLocation(const float32 *x, const float32 *y) {
			_x = x;
			_y = y;
		}
		
		void GameObject::setLocation(const float32 x, const float32 y) {
			_x = new float32(x);
			_y = new float32(y);
		}

		float GameObject::getX() {
			return *_x;
		}

		float GameObject::getY() {
			return *_y;
		}

		float GameObject::getPixelX() { return *_x * 20.0f; }
		float GameObject::getPixelY() { return *_y * 20.0f; }

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
