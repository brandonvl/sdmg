//
//
//  @ Project : SDMG
//  @ File Name : Joystick.h
//  @ Date : 30-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "sdl\include\SDL.h"
#include "sdl\include\SDL_thread.h"
#include <string>
#include <map>
#include <vector>

namespace sdmg {
	namespace engine {
		class Engine;

		namespace input {
			class Joystick {
			public:
				Joystick(int id = 255, std::string name = "Joystick");
				~Joystick();

				int ID;
				std::string Name;

				int getID();
				std::string getName();

			private:
				void initialize();
			};
		}
	}
}