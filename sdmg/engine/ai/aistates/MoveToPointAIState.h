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
				class MoveToPointAIState : public AIState
				{
				public:
					MoveToPointAIState(AIMachine &machine) : AIState("moveToPoint", machine) {};
					virtual ~MoveToPointAIState() { };
					void enter(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
					void update(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
					void exit(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
				};
			}
		}
	}
}

