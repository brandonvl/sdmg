//
//
//  @ Project : SDMG
//  @ File Name : MenuDownAction.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "MenuDownAction.h"

namespace sdmg {
	namespace actions {
		bool MenuDownAction::run() {
			// TODO: Implement action
			return true;
		}
		
		Action* MenuDownAction::create() {
			return new MenuDownAction();
		}
		
	}
}
