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
				class JumpingAIState : public AIState
				{
				public:
					JumpingAIState(AIMachine &machine) : AIState("jumping", machine) {};
					virtual ~JumpingAIState() { };
					void enter(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
					void update(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
					void exit(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
				};
			}
		}
	}
}

