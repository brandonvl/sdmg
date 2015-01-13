#include "ControllerInputDeviceBinding.h"

#include "InputDeviceBinding.h"
namespace sdmg {
	namespace engine {
		namespace input {


			ControllerInputDeviceBinding::ControllerInputDeviceBinding(Joystick *controller) : InputDeviceBinding(), _controller(controller)
			{

			}


			ControllerInputDeviceBinding::~ControllerInputDeviceBinding()
			{
				/*clearBindings();

				if (_keyBindings != nullptr){
					delete _keyBindings;
					_keyBindings = nullptr;
				}*/
			}

			bool ControllerInputDeviceBinding::hasBinding(SDL_Event &event) {
				if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP) {
					return _controller->getID() == event.cbutton.which && _keyBindings->count(event.cbutton.button);
				}

				return false;
			}

			void ControllerInputDeviceBinding::setKeyBinding(const int keyCode, Action *action) {
				// check if binding exists
				if (_keyBindings->count(keyCode)) { delete (*_keyBindings)[keyCode]; (*_keyBindings)[keyCode] = action; } // replace
				else _keyBindings->insert(std::pair<const int, Action*>(keyCode, action)); // insert
			}

			void ControllerInputDeviceBinding::clearBindings() {
				if (_keyBindings != nullptr) {
					for (auto it : *_keyBindings)
					{
						delete it.second;
					}

					_keyBindings->clear();
				}
			}

			Action* ControllerInputDeviceBinding::createAction(SDL_Event &action) {
				// check if keycode exists, create and return the action
				if (_keyBindings->count(action.cbutton.button)) {
					return (*_keyBindings)[action.cbutton.button]->create(action);
				}
				return nullptr;
			}
		}
	}
};
