//
//
//  @ Project : SDMG
//  @ File Name : PhysicsEngine.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once

namespace sdmg {
	namespace engine {
		class Engine;

		namespace physics {
			class PhysicsEngine {
			public:
				void update();
				void pause();
				void resume();
			private:
				Engine *_engine;
				bool _enabled;
			};
		}
	}
}