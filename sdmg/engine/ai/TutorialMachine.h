#pragma once
#include "AIMachine.h"
#include "Box2D\Box2D.h"

namespace sdmg {

	namespace model {
		class Character;
	}

	namespace engine {
		namespace ai {

			namespace aistate {
				class AIState;
			}

			class TutorialMachine : public AIMachine
			{
			public:
				TutorialMachine(model::Character &controlled, model::Character &enemy, const float &moveX, const float &moveY);
				virtual ~TutorialMachine();
			};
		}
	}
};

