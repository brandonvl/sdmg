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
				class MoveLeftAIState : public AIState
				{
					public:
					MoveLeftAIState(AIMachine &machine) : AIState("moveLeft", machine) {};
					virtual ~MoveLeftAIState() { }
					void update(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
				};
			}
		}
	}
}