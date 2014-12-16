#pragma once
#include "AIMachine.h"
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
				TutorialMachine(model::Character &controlled, model::Character &enemy);
				virtual ~TutorialMachine();
			};
		}
	}
};

