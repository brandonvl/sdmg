//
//
//  @ Project : SDMG
//  @ File Name : InputEngine.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#include <iostream>
#include <cmath>
#include "InputEngine.h"
#include "sdl\include\SDL.h"
#include "sdl\include\SDL_thread.h"

namespace sdmg {
	namespace engine {
		namespace input {
			InputEngine::InputEngine() {
				initialize();

				// add controller mappings
				SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");
				findJoysticks();
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

				if (event.type == SDL_CONTROLLERBUTTONUP || event.type == SDL_CONTROLLERBUTTONDOWN) {
					handleKey(Joysticks[event.cbutton.which].Name, event);
				}

				if (event.type == SDL_JOYAXISMOTION)
				{
					if (event.jaxis.value < -abs(JOYSTICK_DEAD_ZONE) || (event.jaxis.value > JOYSTICK_DEAD_ZONE)) {
						
						event.type = SDL_CONTROLLERBUTTONDOWN;

						if (event.jaxis.axis == 0)
						{
							if (event.jaxis.value < -abs(JOYSTICK_DEAD_ZONE))
							{
								// LEFT
								event.cbutton.button = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
							}
							else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
							{
								// RIGHT
								event.cbutton.button = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
							}
						}

						if (event.jaxis.axis == 1)
						{
							if (event.jaxis.value < -abs(JOYSTICK_DEAD_ZONE))
							{
								// DOWN
								event.cbutton.button = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
							}
							else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
							{
								// UP
								event.cbutton.button = SDL_CONTROLLER_BUTTON_DPAD_UP;
							}
						}
					}
					handleKey(Joysticks[event.cbutton.which].Name, event);
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
				if (SDL_Init(SDL_INIT_JOYSTICK) < 0)
				{
					printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
				}

				printf("%i joysticks were found.\n\n", SDL_NumJoysticks());
				printf("The joysticks are:\n");

				// loop through joysticks
				for (int i = 0; i < SDL_NumJoysticks(); i++)
				{
					// check if joystick is a mapped controller
					if (SDL_IsGameController(i))
					{
						printf("Index \'%i\' is a compatible controller, named \'%s\'\n", i, SDL_GameControllerNameForIndex(i));

						if (!Joysticks[i].Stick)
						{
							// set values
							Joysticks[i].ID = i;
							Joysticks[i].Stick = SDL_JoystickOpen(i);
							if (Joysticks[i].Stick)
							{
								const char *joystick_name = SDL_JoystickName(Joysticks[i].Stick);
								Joysticks[i].Name = joystick_name ? joystick_name : "Joystick";
							}
						}
					}
					else
					{
						printf("Index \'%i\' is not a compatible controller.\n", i);
					}
				}
			}
		}
	}
}
