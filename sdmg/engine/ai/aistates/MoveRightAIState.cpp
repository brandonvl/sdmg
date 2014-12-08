#include "MoveRightAIState.h"
#include "model\Character.h"
#include "engine\GameBase.h"
#include "engine\GameTime.h"
#include "engine\MovableGameObject.h"
#include "engine\ai\AIMachine.h"
#include "engine\World.h"
#include <iostream>

namespace sdmg {
	namespace engine {
		namespace ai {
			namespace aistate {

				void MoveRightAIState::update(model::Character &controlled, GameTime &gameTime, GameBase &game)
				{
					MovableGameObject *enemy = game.getWorld()->getPlayers()[1];

					if (enemy->getX() == controlled.getX()){
						_machine->setState("idle");
						return;
					}
					else if (enemy->getX() < controlled.getX()) {
						_machine->setState("moveLeft");
						return;
					}

					if ((controlled.getState() != MoveObjState::WALKING || controlled.getDirection() != MoveObjDirection::RIGHT)) {
						controlled.setState(MoveObjState::WALKING);
						controlled.setDirection(MoveObjDirection::RIGHT);
					}
				}


			}
		}
	}
}