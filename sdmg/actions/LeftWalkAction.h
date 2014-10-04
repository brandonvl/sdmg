//
//
//  @ Project : SDMG
//  @ File Name : LeftWalkAction.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "CharacterAction.h"

namespace sdmg {
	namespace actions {
		class LeftWalkAction : public CharacterAction {
		public:
			LeftWalkAction(Character *character);
			bool run(engine::GameBase &game) override;
			Action* create(SDL_Event &event) override;
		private:
			LeftWalkAction(Character *character, SDL_Event event);
		};
	}
}