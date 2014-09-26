//
//
//  @ Project : SDMG
//  @ File Name : RightWalkAction.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "RightWalkAction.h"

namespace sdmg {
	namespace actions {
		bool RightWalkAction::run() {
			
			// TODO: Implement action
			return true;
		}
		
		Action* RightWalkAction::create() {
			return new RightWalkAction();
		}
		
	}
}
