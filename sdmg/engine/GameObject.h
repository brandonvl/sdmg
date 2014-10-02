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
		enum class Flags;
		class GameTime;
		class GameBase;
		struct Size
		{
			Size::Size() { }
			Size::Size(int width, int height)
			{
				this->width = width;
				this->height = height;
			}
			int width, height;
		};

		class GameObject {
		public:
			GameObject();
			virtual ~GameObject();
			void setId(int id);
			int getId();
			virtual void update(GameTime *gameTime, GameBase *game) = 0;
			std::string getSpriteName();
			b2Body* getBody();
			void setBody(b2Body *body);
			Size getSize();
			int getWidth();
			int getHeight();
			void setSize(Size size);
			void setSize(int width, int height);
		protected:
			b2Body* createBody(b2BodyDef *bodyDef);
		private:
			int _id;
			Flags _flags;
			std::string _spriteName;
			b2Body *_body;
			Size _size;
		};
	}
}