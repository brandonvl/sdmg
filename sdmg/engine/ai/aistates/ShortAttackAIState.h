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
				class ShortAttackAIState : public AIState
				{
				public:
					ShortAttackAIState(AIMachine &machine) : AIState("shortAttack", machine) {};
					virtual ~ShortAttackAIState() { }
					void enter(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
					void update(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
					void exit(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
				};
			}
		}
	}
}

