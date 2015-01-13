#pragma once
#include "InputDeviceBinding.h"
#include "Joystick.h"
namespace sdmg {
	namespace engine {
		class MovableGameObject;

		namespace input {
			class ControllerInputDeviceBinding :
				public InputDeviceBinding
			{
				public:
					ControllerInputDeviceBinding(Joystick *controller);
					virtual ~ControllerInputDeviceBinding();
					virtual void setKeyBinding(const int keyCode, Action *action) override;
					virtual void clearBindings() override;
					virtual bool hasBinding(SDL_Event &event) override;
					virtual Action* createAction(SDL_Event &action) override;
			private:
				Joystick *_controller;
			};
		}
	}
};

