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
				Joysticks = new std::vector<Joystick*>;
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

			std::string InputEngine::getUsedControllerName(SDL_Event &event)
			{
				for (int i = 0; i < Joysticks->size(); i++)
				{
					if (Joysticks->at(i)->getID() == event.jbutton.which)
					{
						if (SDL_IsGameController(i))
						{
							std::cout << Joysticks->at(i)->getName() << std::endl;
							return Joysticks->at(i)->getName();
						}
						//printf("Index \'%i\' is a compatible controller, named \'%s\'\n", Joysticks->at(i)->getID(), Joysticks->at(i)->getName());
					}
				}

				return "keyboard";
			}

			bool InputEngine::handleControllers(SDL_Event &event) {
				SDL_JoystickUpdate();

				if (event.type == SDL_JOYBUTTONDOWN || event.type == SDL_JOYBUTTONUP) {
					printf("Joystick %d button %d down\n",
						event.jbutton.which,
						event.jbutton.button);

					getUsedControllerName(event);

					//printf("Joystick %d", SDL_GameControllerNameForIndex(event.jbutton.which));
					if (event.type == SDL_JOYBUTTONDOWN)
						event.type = SDL_KEYDOWN;

					if (event.type == SDL_JOYBUTTONUP)
						event.type = SDL_KEYUP;

					std::string formatted = std::to_string(event.jbutton.button);
					if (!formatted.empty() && formatted[formatted.size() - 1] == '\r')
					{
						formatted.erase(formatted.size() - 1);
					}

					event.key.keysym.sym = atoi(formatted.c_str());

					handleKey(getUsedControllerName(event), event);
					return true;
				}

				//if (event.type == SDL_JOYAXISMOTION)
				//{
				//	if (event.jaxis.value < 0 && (int)event.jaxis.value > -abs(JOYSTICK_DEAD_ZONE))
				//	{
				//		event.type = SDL_KEYUP;
				//		event.key.keysym.sym = 2;
				//		handleKey(Joysticks->at(event.jbutton.which)->Name, event);
				//		return true;
				//	}
				//	else if (event.jaxis.value > 0 && event.jaxis.value < JOYSTICK_DEAD_ZONE)
				//	{
				//		event.type = SDL_KEYUP;
				//		event.key.keysym.sym = 3;
				//		handleKey(Joysticks->at(event.jbutton.which)->Name, event);
				//		return true;
				//	}

				//	if (event.jaxis.value < -abs(JOYSTICK_DEAD_ZONE) || (event.jaxis.value > JOYSTICK_DEAD_ZONE)) {

				//		if (event.jaxis.axis == 0)
				//		{
				//			if (event.jaxis.value < -abs(JOYSTICK_DEAD_ZONE))
				//			{
				//				// LEFT
				//				event.type = SDL_KEYDOWN;
				//				event.key.keysym.sym = 2;
				//				handleKey(Joysticks->at(event.jbutton.which)->Name, event);
				//				return true;
				//			}
				//			else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
				//			{
				//				// RIGHT
				//				event.type = SDL_KEYDOWN;
				//				event.key.keysym.sym = 3;
				//				handleKey(Joysticks->at(event.jbutton.which)->Name, event);
				//				return true;
				//			}
				//		}
				//		if (event.jaxis.axis == 1)
				//		{
				//			if (event.jaxis.value < -abs(JOYSTICK_DEAD_ZONE))
				//			{
				//				// DOWN
				//				event.key.keysym.sym = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
				//			}
				//			else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
				//			{
				//				// UP
				//				event.key.keysym.sym = SDL_CONTROLLER_BUTTON_DPAD_UP;
				//			}
				//		}
				//	}
				//	//event.key.keysym.sym = event.jbutton.button;
				//	
				//}
				return false;
			}

			void InputEngine::handleEvent(SDL_Event &event) {
				if (event.type == SDL_KEYUP || event.type == SDL_KEYDOWN) {
					handleKey("keyboard", event);
				}

				if (_mouseEnabled) _mouse.handleMouseEvent(event);
			}

			const std::vector<Action*> *InputEngine::getActions() {
				return _actions;
			}

			void InputEngine::update(GameBase &game) {
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

			void InputEngine::pushAction(Action &action, bool keyDown) {

				SDL_Event event = SDL_Event();
				event.type = keyDown ? SDL_KEYDOWN : SDL_KEYUP;

				_actions->push_back(action.create(event));
			}
			
			void InputEngine::clearBindings() {

				for (auto it : *_deviceBindings)
				{
					delete it.second;
				}

				_deviceBindings->clear();
				_mouse.clear();
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
						SDL_Joystick *joy = SDL_JoystickOpen(i);
						
						printf("Index \'%i\' is a compatible controller, named \'%s\'\n", SDL_JoystickInstanceID(joy), SDL_GameControllerNameForIndex(i));
						Joystick *js = new Joystick(SDL_JoystickInstanceID(joy), joy, SDL_GameControllerNameForIndex(i));
						(Joysticks)->push_back(js);
						
						std::cout << SDL_GameControllerNameForIndex(i) << std::endl;
						
						SDL_JoystickEventState(SDL_ENABLE);
					}
					else
					{
						Joystick *js = new Joystick(i, NULL, "None");
						(Joysticks)->push_back(js);
						printf("Index \'%i\' is not a compatible controller.\n", i);
					}
				}
			}
		}
	}
}
