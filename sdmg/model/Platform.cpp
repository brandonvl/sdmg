//
//
//  @ Project : SDMG
//  @ File Name : Platform.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "Platform.h"
#include "engine\GameTime.h"
#include "engine\GameBase.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace model {

		Platform::Platform(bool isAttack)
		{
			_isAttack = isAttack;
		}

		Platform::~Platform() { }

		void Platform::update(GameTime *gameTime, GameBase *game) {
		
		}
		
		bool Platform::getIsAttack()
		{
			return _isAttack;
		}


	}
}
