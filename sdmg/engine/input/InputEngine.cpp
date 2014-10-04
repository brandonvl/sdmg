//
//
//  @ Project : SDMG
//  @ File Name : InputEngine.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "InputEngine.h"
#include "sdl\include\SDL.h"
#include "sdl\include\SDL_thread.h"

namespace sdmg {
	namespace engine {
		namespace input {
			InputEngine::InputEngine() {
				initialize();
			}

			InputEngine::~InputEngine() {
				delete _deviceBindings;
				delete _actions;
			}

			void InputEngine::initialize() {
				_deviceBindings = new std::map<std::string, InputDeviceBinding*>;
				_actions = new std::vector<Action*>;
				_active = true;
			}


			void InputEngine::handleEvent(SDL_Event &event) {
				if (event.type == SDL_KEYUP || event.type == SDL_KEYDOWN) {
					handleKey("keyboard", event);
				}
			}

			const std::vector<Action*> *InputEngine::getActions() {
				return _actions;
			}

			void InputEngine::runActions(GameBase &game) {
				for each (Action *action in (*_actions)) {
					action->run(game);					
					delete action;
				}
				_actions->clear();
			}

			void InputEngine::handleKey(const std::string device, SDL_Event &event) {
				// check if device binding exists
				if (_deviceBindings->count(device)) {
					// if found create and add action 
					Action *action = (*_deviceBindings)[device]->createAction(event);

					if (action != nullptr) {
						_actions->push_back(action);
					}
				}
			}
			
			void InputEngine::setDeviceBinding(std::string device, InputDeviceBinding *binding) {
				// check if device binding exists
				if (_deviceBindings->count(device)) (*_deviceBindings)[device] = binding; // replace
				else _deviceBindings->insert(std::pair<std::string, InputDeviceBinding*>(device, binding)); // insert
			}
			
			void InputEngine::clearBindings() {
				_deviceBindings->clear();
			}

			// find joysticks and add to map
			void InputEngine::findJoysticks(void)
			{
				for (int i = 0; i < SDL_NumJoysticks(); i++)
				{
					if (SDL_IsGameController(i))
					{
						if (!Joysticks[i].Stick)
						{
							Joysticks[i].ID = i;
							Joysticks[i].Stick = SDL_JoystickOpen(i);
							if (Joysticks[i].Stick)
							{
								const char *joystick_name = SDL_JoystickName(Joysticks[i].Stick);
								Joysticks[i].Name = joystick_name ? joystick_name : "Joystick";
							}
						}
					}
				}
			}
		}
	}
}
