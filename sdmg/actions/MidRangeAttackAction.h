//
//
//  @ Project : SDMG
//  @ File Name : MidRangeAttackAction.h
//  @ Date : 21-10-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "CharacterAction.h"

namespace sdmg {
	namespace actions {
		class MidRangeAttackAction : public CharacterAction {
		public:
			MidRangeAttackAction(Character *character);
			bool run(engine::GameBase &game) override;
			Action* create(SDL_Event &event) override;
		private:
			MidRangeAttackAction(Character *character, SDL_Event event);
		};
	}
}