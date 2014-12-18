//
//
//  @ Project : SDMG
//  @ File Name : Platform.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "engine\GameObject.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace engine {
		class GameTime;
		class GameBase;
	}

	namespace model {
		class Platform : public GameObject {
		public:
			Platform();
			virtual ~Platform();
			void update(GameTime *gameTime, GameBase *game);

			// bool getIsAttack();
			int getDamageOnImpact();
			void setDamageOnImpact(int damage);

		protected:
			bool _isAttack;
			int _damageOnImpact;
		};
	}
}