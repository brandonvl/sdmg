#include "LongAttackAIState.h"
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
#include "actions\LongRangeAttackAction.h"

namespace sdmg {
	namespace engine {
		namespace ai {
			namespace aistate {

				void LongAttackAIState::enter(model::Character &controlled, GameTime &gameTime, GameBase &game) {

					if (_transition != "")
						_transition = "";

					_commandSend = false;
				}

				void LongAttackAIState::update(model::Character &controlled, GameTime &gameTime, GameBase &game)
				{
					if (!_commandSend) {
						actions::LongRangeAttackAction action = actions::LongRangeAttackAction(&controlled);
						game.getEngine()->getInputEngine()->pushAction(action);
						_commandSend = true;
						return;
					}

					if (controlled.stateIsInterruptible() && _commandSend)
						_machine->revertToPreviousState(&gameTime, &game);
				}

				void LongAttackAIState::exit(model::Character &controlled, GameTime &gameTime, GameBase &game) {
					_transition = "";
					_commandSend = false;
				}
			}
		}
	}
}
