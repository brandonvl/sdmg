#include "engine\ai\aistates\ShortAttackAIState.h"
#include "engine\ai\AIMachine.h"
#include "model\Character.h"
#include "engine\MovableGameObject.h"
#include "engine\GameBase.h"
#include "engine\GameTime.h"
#include "helperclasses\RandomGenerator.h"
#include "engine\World.h"
#include "engine\Engine.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\physics\RayCastCallBack.h"
#include "engine\input\InputEngine.h"
#include "actions\MidRangeAttackAction.h"

namespace sdmg {
	namespace engine {
		namespace ai {
			namespace aistate {

				void ShortAttackAIState::enter(model::Character &controlled, GameTime &gameTime, GameBase &game) {

					if (_transition != "")
						_transition = "";

					_commandSend = false;
				}

				void ShortAttackAIState::update(model::Character &controlled, GameTime &gameTime, GameBase &game)
				{
					if (!_commandSend) {
						//controlled.setState(MoveObjState::WALKING | MoveObjState::MIDRANGEATTACKBEGIN);
						actions::MidRangeAttackAction action = actions::MidRangeAttackAction(&controlled);
						game.getEngine()->getInputEngine()->pushAction(action);
						_commandSend = true;
						return;
					}

					if (controlled.stateIsInterruptible() && _commandSend)
						_machine->revertToPreviousState(&gameTime, &game);
				}

				void ShortAttackAIState::exit(model::Character &controlled, GameTime &gameTime, GameBase &game) {
					_transition = "";
					_commandSend = false;
				}
			}
		}
	}
}
