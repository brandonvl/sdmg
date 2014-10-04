//
//
//  @ Project : SDMG
//  @ File Name : RightWalkAction.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "CharacterAction.h"

namespace sdmg {
	namespace actions {
		class RightWalkAction : public CharacterAction {
		public:
			RightWalkAction(Character *character);
			bool run(engine::GameBase &game) override;
			Action* create(SDL_Event &event) override;
		private:
			RightWalkAction(Character *character, SDL_Event event);
		};
	}
}