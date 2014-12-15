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
				class MoveRightAIState : public AIState
				{
					public:
					MoveRightAIState(AIMachine &machine) : AIState("moveRight", machine) {};
					virtual ~MoveRightAIState() { }
					void enter(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
					void update(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
					void exit(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
				};
			}
		}
	}
}