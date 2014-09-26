//
//
//  @ Project : SDMG
//  @ File Name : LeftWalkAction.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "engine\input\Action.h"

using namespace sdmg::engine::input;

namespace sdmg {
	namespace actions {
		class LeftWalkAction : public Action {
		public:
			bool run();
			Action* create();
		};
	}
}