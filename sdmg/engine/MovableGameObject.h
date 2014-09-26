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

		class MovableGameObject : public GameObject {
		public:
			void update(GameTime *gameTime, GameBase *game);
		private:
			float _speed;
			Direction _direction;
		};
	}
}