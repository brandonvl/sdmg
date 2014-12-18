#include "TutorialIdleAIState.h"
#include "engine\ai\AIMachine.h"
#include "model\Character.h"

namespace sdmg {
	namespace engine {
		namespace ai {
			namespace aistate {

				TutorialIdleAIState::TutorialIdleAIState(AIMachine &machine, const float *movetoX, const float *movetoY) : AIState("tutorialIdle", machine) {
					if (movetoX != nullptr && movetoY != nullptr)
						_pointToMoveTo = new b2Vec2(*movetoX, *movetoY);
				}

				void TutorialIdleAIState::enter(model::Character &controlled, GameTime &gameTime, GameBase &game) {
					if (_transition != "")
						_transition = "";
				}

				void TutorialIdleAIState::update(model::Character &controlled, GameTime &gameTime, GameBase &game) {

					if (_pointToMoveTo && abs(controlled.getX() - _pointToMoveTo->x) > 1.0F) {
						_transition = "moveToPoint";
					}
					else
					{
						if (controlled.getState() != MoveObjState::IDLE) {
							controlled.setState(MoveObjState::IDLE);
						}
					}

				}

				void TutorialIdleAIState::exit(model::Character &controlled, GameTime &gameTime, GameBase &game) {
					if (_transition != "")
						_transition = "";
				}
			}
		}
	}
}