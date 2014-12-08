#include "MoveLeftAIState.h"
#include "model\Character.h"
#include "engine\GameBase.h"
#include "engine\GameTime.h"
#include "engine\ai\AIMachine.h"
#include "engine\World.h"

namespace sdmg {
	namespace engine {
		namespace ai {
			namespace aistate {

				void MoveLeftAIState::update(model::Character &controlled, GameTime &gameTime, GameBase &game)
				{
					MovableGameObject *enemy = game.getWorld()->getPlayers()[1];

					if (enemy->getX() > controlled.getX()) {
						_machine->setState("moveRight");
						return;
					}

					if ((controlled.getState() != MoveObjState::WALKING || controlled.getDirection() != MoveObjDirection::LEFT)) {
						controlled.setState(MoveObjState::WALKING);
						controlled.setDirection(MoveObjDirection::LEFT);
					}
				}
			}
		}
	}
}