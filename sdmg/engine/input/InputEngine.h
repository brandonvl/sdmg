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
				SDL_Thread *_thread;
				void initialize();
				int waitForKeyInput(void *data);
				void handleKey(std::string device, SDL_Keycode keyCode);
			};
		}
	}
}