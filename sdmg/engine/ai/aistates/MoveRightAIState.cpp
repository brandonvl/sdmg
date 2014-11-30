#include "MoveRightAIState.h"
#include "model\Character.h"
#include "engine\GameBase.h"
#include "engine\GameTime.h"
#include "engine\MovableGameObject.h"
#include <iostream>

namespace sdmg {
	namespace engine {
		namespace ai {
			namespace aistate {

				void MoveRightAIState::update(model::Character &controlled, GameTime &gameTime, GameBase &game)
				{
					if ((controlled.getState() != MoveObjState::WALKING || controlled.getDirection() != MoveObjDirection::RIGHT) && controlled.getState() == MoveObjState::IDLE) {
						controlled.setState(MoveObjState::WALKING);
						controlled.setDirection(MoveObjDirection::RIGHT);
					}
				}


			}
		}
	}
}