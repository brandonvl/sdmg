#include "MoveLeftAIState.h"
#include "model\Character.h"
#include "engine\GameBase.h"
#include "engine\GameTime.h"
#include "engine\ai\AIMachine.h"
#include "engine\World.h"
#include "engine\Engine.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\physics\RayCastCallBack.h"
#include "engine\input\InputEngine.h"
#include "actions\LeftWalkAction.h"

namespace sdmg {
	namespace engine {
		namespace ai {
			namespace aistate {

				void MoveLeftAIState::enter(model::Character &controlled, GameTime &gameTime, GameBase &game) {

					if (!_platformRayCast)
						_platformRayCast = new RayCastCallBack(controlled.getX() - 2.0F, controlled.getY(), controlled.getX() - 2.0F, 1500.0F, nullptr);

					if (!_enemyDetectionRayCast)
						_enemyDetectionRayCast = new RayCastCallBack(controlled.getX(), controlled.getY(), controlled.getX(), controlled.getY(), nullptr);

					if (_transition != "")
						_transition = "";

				}

				void MoveLeftAIState::update(model::Character &controlled, GameTime &gameTime, GameBase &game)
				{

					if (_machine->getEnemy()->getY() < controlled.getY() && _machine->getEnemy()->getX() - 4.0F <= controlled.getX())
					{
						_transition = "jumping";
						return;
					}
					else if (_machine->getEnemy()->getX() - 4.0F > controlled.getX())
					{
						_transition = "moveRight";
						return;
					}

					_platformRayCast->clearResults();
					_platformRayCast->setPointOne(controlled.getX() - 2.0F, controlled.getY());
					_platformRayCast->setPointTwo(controlled.getX() - 2.0F, 1500.0F);

					_enemyDetectionRayCast->clearResults();
					_enemyDetectionRayCast->setPointOne(controlled.getX(), controlled.getY());
					_enemyDetectionRayCast->setPointTwo(controlled.getX() - 3.0F, controlled.getY());

					game.getEngine()->getPhysicsEngine()->performRayCast(*_platformRayCast);
					game.getEngine()->getPhysicsEngine()->performRayCast(*_enemyDetectionRayCast);

					if (_enemyDetectionRayCast->getResults().size() > 0 && _enemyDetectionRayCast->getResults()[0] == _machine->getEnemy()->getBody()) {
						_transition = "shortAttack";
					}
					else if (_platformRayCast->getResults().size() > 0 && _platformRayCast->getResults()[0]->GetType() == b2_staticBody){
						
						if ((controlled.getState() != MoveObjState::WALKING || controlled.getDirection() != MoveObjDirection::LEFT) && controlled.stateIsInterruptible()) {
							actions::LeftWalkAction action = actions::LeftWalkAction(&controlled);
							game.getEngine()->getInputEngine()->pushAction(action);
						}
					}
					else {
						_transition = "moveRight";
					}
				}

				void MoveLeftAIState::exit(model::Character &controlled, GameTime &gameTime, GameBase &game) {
					_transition = "";
				}

				MoveLeftAIState::~MoveLeftAIState() {
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