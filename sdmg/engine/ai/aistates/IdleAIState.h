#pragma once

#include "AIState.h"
#include "Box2D\Box2D.h"
#include <iostream>

namespace sdmg {

	namespace model {
		class Character;
	}

	namespace engine {
		class GameTime;
		class GameBase;

		namespace ai {
			namespace aistate {
				class IdleAIState : public AIState
				{
					public:
					IdleAIState(AIMachine &machine) : AIState("idle", machine) {};
					virtual ~IdleAIState() { };
					void enter(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
					void update(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
					void exit(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
					private:
						float32 testBack(b2Body *body);
				};
			}
		}
	}
}