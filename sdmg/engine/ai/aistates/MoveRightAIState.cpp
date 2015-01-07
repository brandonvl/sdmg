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
#include "engine\input\InputEngine.h"
#include "actions\RightWalkAction.h"
#include "helperclasses\RandomGenerator.h"

namespace sdmg {
	namespace engine {
		namespace ai {
			namespace aistate {

				void MoveRightAIState::enter(model::Character &controlled, GameTime &gameTime, GameBase &game) {

					if (!_platformRayCast)
						_platformRayCast = new RayCastCallBack(controlled.getX() - 2.0F, controlled.getY(), controlled.getX() - 2.0F, 1500.0F, nullptr);

					if (!_enemyDetectionRayCast)
						_enemyDetectionRayCast = new RayCastCallBack(controlled.getX(), controlled.getY(), controlled.getX(), controlled.getY(), nullptr);

					_transition = "";
				}

				void MoveRightAIState::update(model::Character &controlled, GameTime &gameTime, GameBase &game)
				{
					int number = game.getRandomGenerator()->random(1, 100);

					if (number > 99 && controlled.getPP() >= controlled.getLongCost()) {
						_transition = "longAttack";
						return;
					}

					if ((_machine->getEnemy()->getY() - controlled.getY()) < -1.0F && _machine->getEnemy()->getX() + 4.0F >= controlled.getX())
					{
						_transition = "jumping";
						return;
					}
					else if (_machine->getEnemy()->getX() + 4.0F < controlled.getX())
					{
						_transition = "moveLeft";
						return;
					}

					_platformRayCast->clearResults();
					_platformRayCast->setPointOne(controlled.getX() + 2.0F, controlled.getY());
					_platformRayCast->setPointTwo(controlled.getX() + 2.0F, 1500.0F);

					_enemyDetectionRayCast->clearResults();
					_enemyDetectionRayCast->setPointOne(controlled.getX(), controlled.getY());
					_enemyDetectionRayCast->setPointTwo(controlled.getX() + 3.0F, controlled.getY());

					game.getEngine()->getPhysicsEngine()->performRayCast(*_platformRayCast);
					game.getEngine()->getPhysicsEngine()->performRayCast(*_enemyDetectionRayCast);

					if (_enemyDetectionRayCast->getResults().size() > 0 && _enemyDetectionRayCast->getResults()[0] == _machine->getEnemy()->getBody()) {
						_transition = "shortAttack";
					}
					else if (_platformRayCast->getResults().size() > 0 && _platformRayCast->getResults()[0]->GetType() == b2_staticBody) {

						if ((controlled.getState() != MoveObjState::WALKING || controlled.getDirection() != MoveObjDirection::RIGHT) && controlled.stateIsInterruptible()) {
							actions::RightWalkAction action = actions::RightWalkAction(&controlled);
							game.getEngine()->getInputEngine()->pushAction(action);
						}
					}
					else
						_transition = "moveLeft";
				}

				void MoveRightAIState::exit(model::Character &controlled, GameTime &gameTime, GameBase &game) {
					if (_transition != "")
						_transition = "";
				}

				MoveRightAIState::~MoveRightAIState() {
					if (_platformRayCast) {
						delete _platformRayCast;
						_platformRayCast = nullptr;
					}

					if (_enemyDetectionRayCast) {
						delete _enemyDetectionRayCast;
						_enemyDetectionRayCast = nullptr;
					}
				}


			}
		}
	}
}