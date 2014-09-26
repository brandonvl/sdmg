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
				void addAction(Action action);
				void setDeviceBinding(std::string device, InputDeviceBinding binding);
				void clearBindings();
			private:
				Engine *_engine;
				std::map<std::string,InputDeviceBinding> _deviceBinding;
				std::vector<Action> _actions;
			};
		}
	}
}