//
//
//  @ Project : SDMG
//  @ File Name : Joystick.cpp
//  @ Date : 30-9-2014
//  @ Author : 42IN13SAd
//
//

#include "Joystick.h"
#include "sdl\include\SDL.h"
#include "sdl\include\SDL_thread.h"

namespace sdmg {
	namespace engine {
		namespace input {
			Joystick::Joystick(int id, SDL_GameController *pad, std::string name)
			{
				_id = id;

				if (name == "")
					name = "controller" + std::to_string(id+1);

				_name = name;
				_pad = pad;
			}

			Joystick::~Joystick() {
			}

			int Joystick::getID()
			{
				return _id;
			}

			int Joystick::getSDLID()
			{
				return _id - 1;
			}

			std::string Joystick::getName() const
			{
				return _name;
			}

			SDL_GameController *Joystick::getController()
			{
				return _pad;
			}

		}
	}
}
