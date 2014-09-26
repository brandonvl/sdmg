//
//
//  @ Project : SDMG
//  @ File Name : JumpAction.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "JumpAction.h"

namespace sdmg {
	namespace actions {
		bool JumpAction::run() {
			// TODO: Implement action
			return true;
		}
		
		Action* JumpAction::create() {
			return new JumpAction();
		}
		
	}
}
