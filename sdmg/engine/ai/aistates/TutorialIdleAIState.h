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
				class TutorialIdleAIState : public AIState
				{
				private:
					b2Vec2 *_pointToMoveTo;
				public:
					TutorialIdleAIState(AIMachine &machine, const float *movetoX = nullptr, const float *movetoY = nullptr);
					virtual ~TutorialIdleAIState() { if (_pointToMoveTo) delete _pointToMoveTo; };
					void enter(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
					void update(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
					void exit(model::Character &controlled, GameTime &gameTime, GameBase &game) override;
				};
			}
		}
	}
}

