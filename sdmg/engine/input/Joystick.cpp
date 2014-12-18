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
			Joystick::Joystick(int id, SDL_Joystick *stick, std::string name)
			{
				ID = id;
				Stick = stick;
				Name = name;
			}

			Joystick::~Joystick() {
			}

			int Joystick::getID()
			{
				return ID;
			}

			std::string Joystick::getName()
			{
				return Name;
			}

		}
	}
}
