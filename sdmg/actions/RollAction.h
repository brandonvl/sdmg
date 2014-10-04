//
//
//  @ Project : SDMG
//  @ File Name : RollAction.h
//  @ Date : 4-10-2014
//  @ Author : 42IN13SAd
//
//


#pragma once
#include "CharacterAction.h"

namespace sdmg {
	namespace actions {
		class RollAction : public CharacterAction
		{
		public:
			RollAction(Character *character);
			bool run(engine::GameBase &game) override;
			Action* create(SDL_Event &event) override;
		private:
			RollAction(Character *character, SDL_Event event);
		};

	}
}