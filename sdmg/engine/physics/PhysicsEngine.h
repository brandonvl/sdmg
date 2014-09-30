//
//
//  @ Project : SDMG
//  @ File Name : PhysicsEngine.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once

class b2World;
namespace sdmg {
	namespace engine {
		class Engine;

		namespace physics {
			class PhysicsEngine {
			public:
				void update();
				void pause();
				void resume();
				void createWorld(const float leftGravity, const float downGravity);
			private:
				Engine *_engine;
				b2World *_world;
				bool _enabled;
			};
		}
	}
}