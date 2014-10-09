//
//
//  @ Project : SDMG
//  @ File Name : InputDeviceBinding.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "InputDeviceBinding.h"

namespace sdmg {
	namespace engine {
		namespace input {
			InputDeviceBinding::InputDeviceBinding() {
				_keyBindings = new std::map<const int, Action*>;
			}

			InputDeviceBinding::~InputDeviceBinding() {
				delete _keyBindings;
			}

			void InputDeviceBinding::setKeyBinding(const int keyCode, Action *action) {
				// check if binding exists
				if (_keyBindings->count(keyCode)) (*_keyBindings)[keyCode] = action; // replace
				else _keyBindings->insert(std::pair<const int, Action*>(keyCode, action)); // insert
			}
			
			void InputDeviceBinding::clearBindings() {
				_keyBindings->clear();
			}
			
			Action* InputDeviceBinding::createAction(SDL_Event &action) {
				// check if keycode exists, create and return the action
				if (_keyBindings->count(action.key.keysym.sym)) {
					return (*_keyBindings)[action.key.keysym.sym]->create(action);
				}
				return nullptr;
			}
		}
	}
}
