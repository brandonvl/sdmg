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
				
				enum class InputType { KEYBOARD, GAMECONTROLLER };
				void setDeviceBinding(std::string device, InputDeviceBinding* binding);
				void clearBindings();
				void handleEvent(SDL_Event &event);
				//bool handleControllers(SDL_Event &event);
				const std::vector<Action*> *getActions();
				void update(GameBase &game);
				void findJoysticks();
				void enableAllDevices();
				void registerGameController(SDL_Event &event);
				void removeGameController(SDL_Event &event);
				void enableDevice(const std::string &deviceName);
				void disableDevice(const std::string &deviceName);
				void pushAction(Action &action, bool keyDown = true);
				Mouse &getMouse() { return _mouse; }
				void setMouseEnabled(bool enabled = true) { _mouseEnabled = enabled; }
				std::string getUsedControllerName(SDL_Event &event);
				std::map<std::string, InputType> getDevices();
				InputDeviceBinding *createBinding(const std::string &deviceName);
				std::string getDevice(SDL_Event &event);
				std::string getDeviceNameByEvent(SDL_Event &event);
				bool isDeviceEnabled(SDL_Event &event);
			private:
				Engine *_engine = nullptr;
				Mouse _mouse;
				std::map<std::string,InputDeviceBinding*> *_deviceBindings = nullptr;
				
				std::map<std::string, InputType> _devices;
				std::vector<Action*> *_actions = nullptr;
				bool _active, _mouseEnabled;
				std::vector<Joystick*> _gameControllers;
				void initialize();
				void handleKey(const std::string device, SDL_Event &event);
				void addAction(Action &action, bool keyDown);
			};
		}
	}
}