#pragma once
#include "AIState.h"

namespace sdmg {

	namespace model {
		class Character;
	}

	namespace engine {
		class GameTime;
		class GameBase;
		namespace physics {
			class RayCastCallBack;
		}
		namespace ai {
			namespace aistate {
				class MoveLeftAIState : public AIState
				{
					public:
					MoveLeftAIState(AIMachine &machine) : AIState("moveLeft", machine) {};
					virtual ~MoveLeftAIState();
					void enter(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
					void update(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
					void exit(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
				private:
					physics::RayCastCallBack *_platformRayCast;
					physics::RayCastCallBack *_enemyDetectionRayCast;
				};
			}
		}
	}
}