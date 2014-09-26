//
//
//  @ Project : SDMG
//  @ File Name : MenuUpAction.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "MenuUpAction.h"

namespace sdmg {
	namespace actions {
		bool MenuUpAction::run() {			
			// TODO: Implement action
			return true;
		}
		
		Action* MenuUpAction::create() {
			return new MenuUpAction();
		}
		
	}
}
