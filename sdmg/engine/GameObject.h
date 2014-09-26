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

		class GameObject {
		public:
			void setId(int id);
			int getId();
			virtual void update(GameTime *gameTime, GameBase *game) = 0;
			std::string getSpriteName();
			b2Body* getBody();
		protected:
			b2Body* createBody(b2BodyDef *bodyDef);
		private:
			int _id;
			Flags _flags;
			std::string _spriteName;
			b2Body *_body;
		};
	}
}