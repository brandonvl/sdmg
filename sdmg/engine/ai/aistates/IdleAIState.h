#pragma once

#include "AIState.h"

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
					void update(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
				};
			}
		}
	}
}