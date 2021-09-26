//
//
//  @ Project : SDMG
//  @ File Name : Action.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include <SDL.h>
#include <string>

namespace sdmg {
	namespace engine {
		class GameBase;

		namespace input {
			class Action {
			public:
				Action(std::string name) : _name(name) {}
				virtual bool run(GameBase &game) = 0;
				virtual Action* create(SDL_Event &event) = 0;
				const std::string &getName() { return _name; }
			protected:
				SDL_Event _event;
				std::string _name;
				Action(SDL_Event event, std::string name) : _event(event), _name(name) {}
			};
		}
	}
}