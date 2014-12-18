#pragma once
#include "AIState.h"
#include "Box2D\Box2D.h"

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
				private:
					b2Vec2 _pointToMoveTo;
				public:
					MoveToPointAIState(AIMachine &machine, const float &x, const float &y) : AIState("moveToPoint", machine), _pointToMoveTo(x, y) {};
					virtual ~MoveToPointAIState() { };
					void enter(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
					void update(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
					void exit(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
				};
			}
		}
	}
}

