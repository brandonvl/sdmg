//
//
//  @ Project : SDMG
//  @ File Name : Action.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "sdl\include\SDL.h"

namespace sdmg {
	namespace engine {
		class GameBase;

		namespace input {
			class Action {
			public:
				Action() {}
				virtual bool run(GameBase &game) = 0;
				virtual Action* create(SDL_Event &event) = 0;
			protected:
				SDL_Event _event;
				Action(SDL_Event event) : _event(event) {}
			};
		}
	}
}