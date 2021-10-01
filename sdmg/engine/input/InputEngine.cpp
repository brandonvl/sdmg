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
#include "ControllerInputDeviceBinding.h"
#include <SDL.h>
#include <algorithm>

namespace sdmg {
	namespace engine {
		namespace input {
			InputEngine::InputEngine() {
				initialize();

				// add controller mappings
				SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");
				findJoysticks();
				_devices.insert({ "keyboardLEFT", InputType::KEYBOARD });
				_devices.insert({ "keyboardRIGHT", InputType::KEYBOARD });
			}

			InputEngine::~InputEngine() {


				clearBindings();

				for (auto it : *_actions)
				{
					delete it;
				}

				for (auto it : _gameControllers)
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
				for (int i = 0; i < _gameControllers.size(); i++)
				{
					if (_gameControllers.at(i)->getID() == event.jbutton.which)
					{
						if (SDL_IsGameController(i))
						{
							std::cout << _gameControllers.at(i)->getName() << std::endl;
							return _gameControllers.at(i)->getName();
						}
						//printf("Index \'%i\' is a compatible controller, named \'%s\'\n", Joysticks->at(i)->getID(), Joysticks->at(i)->getName());
					}
				}

				return "keyboard";
			}

			InputDeviceBinding *InputEngine::createBinding(const std::string &deviceName) {

				auto result = _devices.find(deviceName);

				if (result != _devices.end()) {

					switch (result->second)
					{
					case InputType::GAMECONTROLLER:
							{
								auto it = std::find_if(_gameControllers.begin(), _gameControllers.end(), [&result](const Joystick *joystick) { return joystick->getName() == result->first; });

								return new ControllerInputDeviceBinding(*it);
								break;
							}		
						default:
							return new InputDeviceBinding();
							break;
					}

				}

				return new InputDeviceBinding();
			}

			void InputEngine::handleEvent(SDL_Event &event) {
				if (event.type == SDL_KEYUP || event.type == SDL_KEYDOWN || event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP) {
					std::string deviceName = getDevice(event);
					auto it = _devices.find(deviceName);
					if(it != _devices.end() && it->second == InputType::GAMECONTROLLER)
						event.type = event.type == SDL_CONTROLLERBUTTONDOWN ? SDL_KEYDOWN : SDL_KEYUP;

					handleKey(deviceName, event);
				}

				if (_mouseEnabled) _mouse.handleMouseEvent(event);
			}

			bool InputEngine::isDeviceEnabled(SDL_Event &event)
			{
				if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
					for (auto &it : *_deviceBindings) {
						if (it.second->isKeyBoard() && it.second->isEnabled())
							return true;
					}
				}
				else if(event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP) {
					for (auto &it : *_deviceBindings) {
						if (!it.second->isKeyBoard() && it.second->isEnabled()){
							if (static_cast<ControllerInputDeviceBinding*>(it.second)->getController()->getID() == event.cbutton.which)
								return true;
						}
					}
				}

				return false;
			}

			void InputEngine::registerGameController(SDL_Event &event)
			{
				if (event.type == SDL_CONTROLLERDEVICEADDED) {
					int i = event.cdevice.which;
					if (SDL_IsGameController(i)) {
						SDL_GameController *pad = SDL_GameControllerOpen(i);

						if (pad) {
							std::cout << "I found a new device! = " << i << "\n";
							SDL_Joystick *joy = SDL_GameControllerGetJoystick(pad);
							int instanceID = SDL_JoystickInstanceID(joy);

							// You can add to your own map of joystick IDs to controllers here.
							Joystick *controller = new Joystick{ i, pad };
							_gameControllers.push_back(controller);
							_devices.insert({ controller->getName(), InputType::GAMECONTROLLER });
						}
					}
					

				}

			}

			void InputEngine::removeGameController(SDL_Event &event)
			{
				if (event.type == SDL_CONTROLLERDEVICEREMOVED) {

					Joystick *controller = nullptr;

					std::pair<std::string, InputDeviceBinding*> *selectedBinding = nullptr;

					for (auto &it : *_deviceBindings) {
						if (it.second->hasThisGamePad(event)) {
							*selectedBinding = it;
							controller = static_cast<ControllerInputDeviceBinding*>(it.second)->getController();
							break;
						}
					}

					if (controller == nullptr) {
						for (auto &contr : _gameControllers) {
							if (contr->getID() == event.cdevice.which)
								controller = contr;
						}
					}

					if (selectedBinding != nullptr) {
						_deviceBindings->erase(selectedBinding->first);
						delete selectedBinding;
					}

					if (controller != nullptr) {
						auto pos = std::remove(_gameControllers.begin(), _gameControllers.end(), controller);
						_gameControllers.erase(pos, _gameControllers.end());
						_devices.erase(controller->getName());
						SDL_GameControllerClose(controller->getController());
						delete controller;
					}

				}
			}

			std::string InputEngine::getDevice(SDL_Event &event) {

				for (auto &it : *_deviceBindings) {
					if (it.second->hasBinding(event))
						return it.first;
				}

				return "";
			}

			std::string InputEngine::getDeviceNameByEvent(SDL_Event &event) {

				if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP) {
					for (auto &it : _gameControllers) {
						if (it->getID() == event.cbutton.which)
							return it->getName();
					}
				}

				return "";
			}

			std::map<std::string, InputEngine::InputType> InputEngine::getDevices() {
				return _devices;
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

			void InputEngine::enableDevice(const std::string &deviceName)
			{
				auto device = _deviceBindings->find(deviceName);

				if (device != _deviceBindings->end()) {
					device->second->enable();
				}
			}

			void InputEngine::disableDevice(const std::string &deviceName)
			{
				auto device = _deviceBindings->find(deviceName);

				if (device != _deviceBindings->end()) {
					device->second->disable();
				}
			}

			void InputEngine::handleKey(const std::string device, SDL_Event &event) {
				// check if device binding exists
				if (_deviceBindings->count(device)) {

					InputDeviceBinding *binding = (*_deviceBindings)[device];

					if (binding->isEnabled()) {
						// if found create and add action 
						Action *action = binding->createAction(event);

						if (action != nullptr) {
							addAction(*action, (event.type == SDL_KEYDOWN || event.type == SDL_CONTROLLERBUTTONDOWN));
						}
					}
				}
			}

			void InputEngine::addAction(Action &action, bool keyDown) {
				_actions->push_back(&action);
			}
			
			void InputEngine::setDeviceBinding(std::string device, InputDeviceBinding *binding) {
				// check if device binding exists
				if (_deviceBindings->count(device)) { delete (*_deviceBindings)[device]; (*_deviceBindings)[device] = binding; } // replace
				else _deviceBindings->insert(std::pair<std::string, InputDeviceBinding*>(device, binding)); // insert
			}

			void InputEngine::pushAction(Action &action, bool keyDown) {

				SDL_Event event = SDL_Event();
				event.type = keyDown ? SDL_KEYDOWN : SDL_KEYUP;
				
				addAction(*action.create(event), keyDown);
			}

			void InputEngine::enableAllDevices() {

				for (auto &it : *_deviceBindings) {
					it.second->enable();
				}

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
					if (SDL_IsGameController(i)) {
						SDL_GameController *pad = SDL_GameControllerOpen(i);

						if (pad) {
							std::cout << "I found a new device! = " << i << "\n";
							SDL_Joystick *joy = SDL_GameControllerGetJoystick(pad);
							int instanceID = SDL_JoystickInstanceID(joy);

							// You can add to your own map of joystick IDs to controllers here.
							Joystick *controller = new Joystick{ i, pad };
							_gameControllers.push_back(controller);
							_devices.insert({ controller->getName(), InputType::GAMECONTROLLER });
						}
					}
				}
			}
		}
	}
}
