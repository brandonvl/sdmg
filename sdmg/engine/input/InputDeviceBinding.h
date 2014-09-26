//
//
//  @ Project : SDMG
//  @ File Name : InputDeviceBinding.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "Action.h"
#include "sdl\include\SDL.h"
#include <map>

namespace sdmg {
	namespace engine {
		class MovableGameObject;

		namespace input {
			class InputDeviceBinding {
			public:
				void setKeyBinding(const SDL_Keycode &key, const Action *action);
				void clearBindings();
			private:
				MovableGameObject *_gameObject;
				std::map<SDL_Keycode, Action> _keyBindings;
			};
		}
	}
}