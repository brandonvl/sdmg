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
			InputDeviceBinding::InputDeviceBinding(MovableGameObject *gameObject) : _gameObject(gameObject) {
				_keyBindings = new std::map<const SDL_Keycode, Action*>;
			}

			InputDeviceBinding::~InputDeviceBinding() {
				delete _keyBindings;
			}

			void InputDeviceBinding::setKeyBinding(const SDL_Keycode keyCode, Action *action) {
				// check if binding exists
				if (_keyBindings->count(keyCode)) (*_keyBindings)[keyCode] = action; // replace
				else _keyBindings->insert(std::pair<const SDL_Keycode, Action*>(keyCode, action)); // insert
			}
			
			void InputDeviceBinding::clearBindings() {
				_keyBindings->clear();
			}
			
			Action* InputDeviceBinding::createAction(const SDL_Keycode &keyCode) {
				// check if keycode exists, create and return the action
				if (_keyBindings->count(keyCode)) {
					return (*_keyBindings)[keyCode]->create();
				}
				return nullptr;
			}
		}
	}
}
