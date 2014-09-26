//
//
//  @ Project : SDMG
//  @ File Name : RightWalkAction.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "engine\input\Action.h"

using namespace sdmg::engine::input;

namespace sdmg {
	namespace actions {
		class RightWalkAction : public Action {
		public:
			bool run();
			Action* create();
		};
	}
}