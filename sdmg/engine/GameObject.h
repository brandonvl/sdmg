//
//
//  @ Project : SDMG
//  @ File Name : GameObject.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once 
#include <Box2D\Box2D.h>
#include <string>

namespace sdmg {
	namespace engine {
		class World;
		enum class Flags;
		class GameTime;
		class GameBase;
		struct Size
		{
			Size::Size() { }
			Size::Size(float width, float height)
			{
				this->width = width;
				this->height = height;
			}
			float width, height;
		};

		class GameObject {
		public:
			enum class Flags {
				DRAWABLE = 0x1,
				SHOOTABLE = 0x2,
				CANDIE = 0x4,
				CANDYCANE = 0x8
			};

			GameObject();
			virtual ~GameObject();
			uint32 getId();
			virtual void update(GameTime *gameTime, GameBase *game) = 0;
			std::string getSpriteName();
			b2Body* getBody();
			void setBody(b2Body *body);
			Size getSize();
			int getWidth();
			int getHeight();
			void setSize(Size size);
			void setSize(float width, float height);
			void setLocation(const float32 *x, const float32 *y);
			void setLocation(const float32 x, const float32 y);
			float getX();
			float getY();
			int getPixelX();
			int getPixelY();
			void bindWorld(World *world, int id);
			World *getWorld();
			const Flags &getFlags();

			float getSpawnLocationX();
			float getSpawnLocationY();
			void setSpawnLocation(const float x, const float y);
			virtual void die() {};

			std::string getName();
			void setName(std::string name);
			std::string getLevel();
			void GameObject::setLevel(std::string level);
		protected:
			b2Body* createBody(b2BodyDef *bodyDef);
			b2Body *_body = nullptr;
			b2Vec2 *_spawnLocation = nullptr;
			Flags _flags;
		private:
			uint32 _id;
			std::string _spriteName;
			Size _size;
			const float32 *_x = nullptr, *_y = nullptr;
			float _spawnX, _spawnY;
			World *_world = nullptr;
			std::string _name, _level;
			bool _ownsLocation = false;
		};
	}
}