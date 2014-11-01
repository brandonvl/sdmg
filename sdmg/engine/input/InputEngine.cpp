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


				clearBindings();

				for (auto it : *_actions)
				{
					delete it;
				}

				delete _deviceBindings;
				delete _actions;
			}

			void InputEngine::initialize() {
				_deviceBindings = new std::map<std::string, InputDeviceBinding*>;
				_actions = new std::vector<Action*>;
				_active = true;
			}

			bool InputEngine::handleControllers(SDL_Event &event) {
				SDL_JoystickUpdate();

				if (event.type == SDL_JOYBUTTONDOWN || event.type == SDL_JOYBUTTONUP) {
					printf("Joystick %d button %d down\n",
						event.jbutton.which,
						event.jbutton.button);
					//printf("Joystick %d", SDL_GameControllerNameForIndex(event.jbutton.which));
					if (event.type == SDL_JOYBUTTONDOWN)
						event.type = SDL_KEYDOWN;

					if (event.type == SDL_JOYBUTTONUP)
						event.type = SDL_KEYUP;

					event.key.keysym.sym = event.jbutton.button;
					handleKey(Joysticks[event.jbutton.which].Name, event);
					return true;
				}

				if (event.type == SDL_JOYAXISMOTION)
				{
					if (event.jaxis.value < 0 && (int)event.jaxis.value > -abs(JOYSTICK_DEAD_ZONE))
					{
						event.type = SDL_KEYUP;
						event.key.keysym.sym = 2;
						handleKey(Joysticks[event.jbutton.which].Name, event);
						return true;
					}
					else if (event.jaxis.value > 0 && event.jaxis.value < JOYSTICK_DEAD_ZONE)
					{
						event.type = SDL_KEYUP;
						event.key.keysym.sym = 3;
						handleKey(Joysticks[event.jbutton.which].Name, event);
						return true;
					}

					if (event.jaxis.value < -abs(JOYSTICK_DEAD_ZONE) || (event.jaxis.value > JOYSTICK_DEAD_ZONE)) {

						if (event.jaxis.axis == 0)
						{
							if (event.jaxis.value < -abs(JOYSTICK_DEAD_ZONE))
							{
								// LEFT
								event.type = SDL_KEYDOWN;
								event.key.keysym.sym = 2;
								handleKey(Joysticks[event.jbutton.which].Name, event);
								return true;
							}
							else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
							{
								// RIGHT
								event.type = SDL_KEYDOWN;
								event.key.keysym.sym = 3;
								handleKey(Joysticks[event.jbutton.which].Name, event);
								return true;
							}
						}
						if (event.jaxis.axis == 1)
						{
							if (event.jaxis.value < -abs(JOYSTICK_DEAD_ZONE))
							{
								// DOWN
								event.key.keysym.sym = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
							}
							else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
							{
								// UP
								event.key.keysym.sym = SDL_CONTROLLER_BUTTON_DPAD_UP;
							}
						}
					}
					//event.key.keysym.sym = event.jbutton.button;
					
				}
				return false;
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
				if (_deviceBindings->count(device)) { delete (*_deviceBindings)[device]; (*_deviceBindings)[device] = binding; } // replace
				else _deviceBindings->insert(std::pair<std::string, InputDeviceBinding*>(device, binding)); // insert
			}
			
			void InputEngine::clearBindings() {

				for (auto it : *_deviceBindings)
				{
					delete it.second;
				}

				_deviceBindings->clear();
			}

			// find joysticks and add to map
			void InputEngine::findJoysticks()
			{
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
							std::cout << SDL_GameControllerNameForIndex(i) << std::endl;
							// set values
							Joysticks[i].ID = i;
							Joysticks[i].Stick = SDL_JoystickOpen(i);
							Joysticks[i].Name = SDL_GameControllerNameForIndex(i);
						}
						SDL_JoystickEventState(SDL_ENABLE);
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
