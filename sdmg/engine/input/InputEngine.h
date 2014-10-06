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
				void handleEvent(SDL_Event &event);
				const std::vector<Action*> *getActions();
				void runActions(GameBase &game);
			private:
				Engine *_engine;
				std::map<std::string,InputDeviceBinding*> *_deviceBindings;
				std::vector<Action*> *_actions;
				bool _active;
				//SDL_Event _event;
				//SDL_Thread *_thread;
				const int JOYSTICK_DEAD_ZONE = 3200;
				std::map<Uint8, Joystick> Joysticks;
				void initialize();
				void handleKey(const std::string device, SDL_Event &event);
				void findJoysticks(void);
			};
		}
	}
}