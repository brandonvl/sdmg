#include "TutorialMachine.h"
#include "aistates\TutorialIdleAIState.h"
#include "aistates\MoveToPointAIState.h"

namespace sdmg {

	namespace engine {
		namespace ai {

			TutorialMachine::TutorialMachine(model::Character &controlled, model::Character &enemy, const float &moveX, const float &moveY) : AIMachine(controlled, enemy)
			{
				setDefaultState(new aistate::TutorialIdleAIState(*this, &moveX, &moveY));
				addState(new aistate::MoveToPointAIState(*this, moveX, moveY));
			}


			TutorialMachine::~TutorialMachine()
			{
			}
		}
	}
}

