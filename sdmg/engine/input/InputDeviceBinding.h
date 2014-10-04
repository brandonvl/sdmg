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
				InputDeviceBinding();
				~InputDeviceBinding();
				void setKeyBinding(const SDL_Keycode keyCode, Action *action);
				void clearBindings();
				Action* createAction(SDL_Event &action);
			private:
				std::map<const SDL_Keycode, Action*> *_keyBindings;
			};
		}
	}
}