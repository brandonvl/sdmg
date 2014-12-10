#include "MoveRightAIState.h"
#include "model\Character.h"
#include "engine\GameBase.h"
#include "engine\GameTime.h"
#include "engine\MovableGameObject.h"
#include "engine\ai\AIMachine.h"
#include "engine\World.h"
#include "engine\Engine.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\physics\RayCastCallBack.h"
#include <iostream>

namespace sdmg {
	namespace engine {
		namespace ai {
			namespace aistate {

				void MoveRightAIState::update(model::Character &controlled, GameTime &gameTime, GameBase &game)
				{
					MovableGameObject *enemy = game.getWorld()->getPlayers()[1];

					RayCastCallBack rayCast = RayCastCallBack(controlled.getX() + 2.0F, controlled.getY(), controlled.getX() + 2.0F, 1500.0F, nullptr);
					game.getEngine()->getPhysicsEngine()->performRayCast(rayCast);


					if (rayCast.getResults().size() > 0) {
						/*if (enemy->getX() == controlled.getX()){
						_machine->setState("idle");
						return;
						}
						else if (enemy->getX() < controlled.getX()) {
						_machine->setState("moveLeft");
						return;
						}*/

						if ((controlled.getState() != MoveObjState::WALKING || controlled.getDirection() != MoveObjDirection::RIGHT) && controlled.stateIsInterruptible()) {
							controlled.setState(MoveObjState::WALKING);
							controlled.setDirection(MoveObjDirection::RIGHT);
						}
					}
					else
						_machine->setState("moveLeft");
				}


			}
		}
	}
}