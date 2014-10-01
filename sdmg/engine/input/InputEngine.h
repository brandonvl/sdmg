//
//
//  @ Project : SDMG
//  @ File Name : InputEngine.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "Action.h"
#include "InputDeviceBinding.h"
#include "Joystick.h"
#include <string>
#include <map>
#include <vector>

namespace sdmg {
	namespace engine {
		class Engine;

		namespace input {
			class InputEngine {
			public:
				InputEngine();
				~InputEngine();
				void setDeviceBinding(std::string device, InputDeviceBinding* binding);
				void clearBindings();
			private:
				Engine *_engine;
				std::map<std::string,InputDeviceBinding*> *_deviceBindings;
				std::vector<Action*> *_actions;
				bool _active;
				SDL_Event _event;
				SDL_Thread *_thread;
				const int JOYSTICK_DEAD_ZONE = 8000;
				std::map<Uint8, Joystick> Joysticks;
				void initialize();
				int waitForKeyInput(void *data);
				void handleKey(std::string device, SDL_Keycode keyCode);
				void findJoysticks(void);
			};
		}
	}
}