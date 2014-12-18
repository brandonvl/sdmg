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
				class LongAttackAIState : public AIState
				{
				public:
					LongAttackAIState(AIMachine &machine) : AIState("longAttack", machine) {};
					virtual ~LongAttackAIState() { }
					void enter(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
					void update(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
					void exit(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
				private:
					bool _commandSend = false;
				};
			}
		}
	}
}
