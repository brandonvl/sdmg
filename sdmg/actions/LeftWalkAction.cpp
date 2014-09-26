//
//
//  @ Project : SDMG
//  @ File Name : LeftWalkAction.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "LeftWalkAction.h"

namespace sdmg {
	namespace actions {
		bool LeftWalkAction::run() {
			// TODO: Implement action
			return true;
		}
		
		Action* LeftWalkAction::create() {
			return new LeftWalkAction();
		}
		
	}
}
