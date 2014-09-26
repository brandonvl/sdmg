//
//
//  @ Project : SDMG
//  @ File Name : MenuSelectAction.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "MenuSelectAction.h"

namespace sdmg {
	namespace actions {
		bool MenuSelectAction::run() {
			// TODO: Implement action
			return true;
		}
		
		Action* MenuSelectAction::create() {
			return new MenuSelectAction();
		}
		
	}
}
