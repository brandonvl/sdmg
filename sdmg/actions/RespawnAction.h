//
//
//  @ Project : SDMG
//  @ File Name : RespawnAction.h
//  @ Date : 8-10-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "CharacterAction.h"

namespace sdmg {
	namespace actions {
		class RespawnAction : public CharacterAction {
		public:
			RespawnAction(Character *character);
			bool run(engine::GameBase &game) override;
			Action* create(SDL_Event &event) override;
		private:
			RespawnAction(Character *character, SDL_Event event);
		};
	}
}