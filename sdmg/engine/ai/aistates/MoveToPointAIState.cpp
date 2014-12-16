#include "MoveToPointAIState.h"
#include "model\Character.h"

namespace sdmg {
	namespace engine {
		namespace ai {
			namespace aistate {
				void MoveToPointAIState::enter(model::Character &controlled, GameTime &gameTime, GameBase &game) {
					if (_transition != "")
						_transition = "";
				}

				void MoveToPointAIState::update(model::Character &controlled, GameTime &gameTime, GameBase &game) {

					if (abs(controlled.getX() - _pointToMoveTo.x) < 0.2F) {
						_transition = "tutorialIdle";
					}
					else if (controlled.getX() > _pointToMoveTo.x) {
						if ((controlled.getState() != MoveObjState::WALKING || controlled.getDirection() != MoveObjDirection::LEFT) && controlled.stateIsInterruptible()) {
							controlled.setState(MoveObjState::WALKING);
							controlled.setDirection(MoveObjDirection::LEFT);
						}
					}
					else if (controlled.getX() < _pointToMoveTo.x) {
						if ((controlled.getState() != MoveObjState::WALKING || controlled.getDirection() != MoveObjDirection::RIGHT) && controlled.stateIsInterruptible()) {
							controlled.setState(MoveObjState::WALKING);
							controlled.setDirection(MoveObjDirection::RIGHT);
						}
					}
					else _transition = "tutorialIdle";

				}

				void MoveToPointAIState::exit(model::Character &controlled, GameTime &gameTime, GameBase &game) {
					_transition = "";
				}
			}
		}
	}
}
