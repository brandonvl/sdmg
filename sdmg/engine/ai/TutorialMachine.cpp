#include "TutorialMachine.h"

namespace sdmg {

	namespace engine {
		namespace ai {

			TutorialMachine::TutorialMachine(model::Character &controlled, model::Character &enemy) : AIMachine(controlled, enemy)
			{

			}


			TutorialMachine::~TutorialMachine()
			{
			}
		}
	}
}

