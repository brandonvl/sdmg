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
#include <SDL.h>
#include <map>

namespace sdmg {
	namespace engine {
		class MovableGameObject;

		namespace input {
			class InputDeviceBinding {
			public:
				InputDeviceBinding();
				virtual ~InputDeviceBinding();
				virtual void setKeyBinding(const int keyCode, Action *action);
				virtual bool hasBinding(SDL_Event &event);
				virtual void disable() { _enabled = false; }
				virtual void enable() { _enabled = true; }
				virtual bool isEnabled() { return _enabled; }
				virtual void clearBindings();
				virtual bool isKeyBoard() { return true; }
				virtual bool hasThisGamePad(SDL_Event &event) { return false; }
				virtual Action* createAction(SDL_Event &action);
			protected:
				bool _enabled = true;
				std::map<const int, Action*> *_keyBindings;
			};
		}
	}
}