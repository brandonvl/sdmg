//
//
//  @ Project : SDMG
//  @ File Name : GameObject.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#include "World.h"
#include "GameObject.h"

namespace sdmg {
	namespace engine {
		GameObject::GameObject()
		{
		}

		GameObject::~GameObject()
		{
			if (_ownsLocation) {
				delete _x;
				delete _y;
			}
			_x = nullptr;
			_y = nullptr;
			_world = nullptr;
		}

		uint32 GameObject::getId() {
			return _id;
		}

		const GameObject::Flags &GameObject::getFlags()
		{
			return _flags;
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
			if (_ownsLocation) {
				delete _x;
				delete _y;
			}
			_x = x;
			_y = y;
			_ownsLocation = false;
		}
		
		void GameObject::setLocation(const float32 x, const float32 y) {
			if (_ownsLocation) {
				delete _x;
				delete _y;
			}
			_x = new float32(x);
			_y = new float32(y);
			_ownsLocation = true;
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


		float GameObject::getSpawnLocationX()
		{
			return _spawnX;
		}

		float GameObject::getSpawnLocationY()
		{
			return _spawnY;
		}

		void GameObject::setSpawnLocation(const float x, const float y)
		{
			_spawnX = x;
			_spawnY = y;
		}

		std::string GameObject::getName()
		{
			return _name;
		}

		void GameObject::setName(std::string name)
		{
			_name = name;
		}

		void GameObject::bindWorld(World *world, int id) { _world = world; _id = id; }
		World *GameObject::getWorld() { return _world; }
	}
}
