//
//
//  @ Project : SDMG
//  @ File Name : MovableGameObject.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "GameObject.h"

namespace sdmg {
	namespace engine {
		class GameTime;
		class GameBase;
		enum class Direction;
		struct Speed
		{
			float horizontal, vertical;
		};

		class MovableGameObject : public GameObject {
		public:
			void setSpeed(Speed speed);
			void update(GameTime *gameTime, GameBase *game);
			Speed getSpeed();
		private:
			Direction _direction;
			Speed _speed;
		};
	}
}