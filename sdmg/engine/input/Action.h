//
//
//  @ Project : SDMG
//  @ File Name : Action.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once

namespace sdmg {
	namespace engine {
		namespace input {
			class Action {
			public:
				virtual bool run() = 0;
				virtual Action* create() = 0;
			};
		}
	}
}