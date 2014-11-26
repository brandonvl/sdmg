//
//
//  @ Project : SDMG
//  @ File Name : BlockAction.h
//  @ Date : 11-11-2014
//  @ Author : 42IN13SAd
//
//


#pragma once
#include "CharacterAction.h"

namespace sdmg {
	namespace actions {
		class BlockAction : public CharacterAction {
		public:
			BlockAction(Character *character);
			bool run(engine::GameBase &game) override;
			Action* create(SDL_Event &event) override;
		private:
			BlockAction(Character *character, SDL_Event event);
		};
	}
}