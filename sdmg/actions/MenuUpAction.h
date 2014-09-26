//
//
//  @ Project : SDMG
//  @ File Name : MenuUpAction.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "engine\input\Action.h"

using namespace sdmg::engine::input;

namespace sdmg {
	namespace actions {
		class MenuUpAction : public Action {
		public:
			bool run() override;
			Action* create() override;
		};
	}
}