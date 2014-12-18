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
#include <stack>
#include "Mouse.h"
#include <chrono>

namespace sdmg {
	namespace engine {
		class Engine;

		namespace input {
			class Action;

			class InputEngine {
			public:
				InputEngine();
				~InputEngine();
				void setDeviceBinding(std::string device, InputDeviceBinding* binding);
				void clearBindings();
				void handleEvent(SDL_Event &event);
				bool handleControllers(SDL_Event &event);
				const std::vector<Action*> *getActions();
				void update(GameBase &game);
				void findJoysticks();
				void pushAction(Action &action, bool keyDown = true);
				Mouse &getMouse() { return _mouse; }
				void setMouseEnabled(bool enabled = true) { _mouseEnabled = enabled; }
				std::string getUsedControllerName(SDL_Event &event);
			private:
				Engine *_engine;
				Mouse _mouse;
				std::map<std::string,InputDeviceBinding*> *_deviceBindings;
				std::vector<Action*> *_actions;
				bool _active, _mouseEnabled;
				//SDL_Event _event;
				//SDL_Thread *_thread;
				const int JOYSTICK_DEAD_ZONE = 3200;
				std::vector<Joystick*> *Joysticks;
				void initialize();
				void handleKey(const std::string device, SDL_Event &event);
				void addAction(Action &action, bool keyDown);
			};
		}
	}
}