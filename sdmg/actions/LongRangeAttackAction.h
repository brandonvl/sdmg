//
//
//  @ Project : SDMG
//  @ File Name : LongRangeAttackAction.h
//  @ Date : 12-11-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "CharacterAction.h"

namespace sdmg {
	namespace actions {
		class LongRangeAttackAction : public CharacterAction {
		public:
			LongRangeAttackAction(Character *character);
			bool run(engine::GameBase &game) override;
			Action* create(SDL_Event &event) override;
		private:
			LongRangeAttackAction(Character *character, SDL_Event event);
		};
	}
}