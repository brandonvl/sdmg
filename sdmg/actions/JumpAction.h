//
//
//  @ Project : SDMG
//  @ File Name : JumpAction.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#pragma once
#include "CharacterAction.h"

namespace sdmg {
	namespace actions {
		class JumpAction : public CharacterAction {
		public:
			JumpAction(Character *character);
			bool run(engine::GameBase &game) override;
			Action* create(SDL_Event &event) override;
		private:
			JumpAction(Character *character, SDL_Event event);
		};
	}
}