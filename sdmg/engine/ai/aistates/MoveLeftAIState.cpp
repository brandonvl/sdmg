#include "MoveLeftAIState.h"
#include "model\Character.h"
#include "engine\GameBase.h"
#include "engine\GameTime.h"

namespace sdmg {
	namespace engine {
		namespace ai {
			namespace aistate {

				void MoveLeftAIState::update(model::Character &controlled, GameTime &gameTime, GameBase &game)
				{
					if ((controlled.getState() != MoveObjState::WALKING || controlled.getDirection() != MoveObjDirection::LEFT) && controlled.getState() == MoveObjState::IDLE) {
						controlled.setState(MoveObjState::WALKING);
						controlled.setDirection(MoveObjDirection::LEFT);
					}
				}
			}
		}
	}
}