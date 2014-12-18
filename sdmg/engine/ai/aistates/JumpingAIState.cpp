#include "JumpingAIState.h"
#include "engine\ai\AIMachine.h"
#include "model\Character.h"
#include "engine\GameBase.h"
#include "engine\GameTime.h"
#include "engine\input\InputEngine.h"
#include "actions\JumpAction.h"
#include "engine\Engine.h"

#include <iostream>

namespace sdmg {
	namespace engine {
		namespace ai {
			namespace aistate {

				void JumpingAIState::enter(model::Character &controlled, GameTime &gameTime, GameBase &game) {

					if (_transition != "")
						_transition = "";
				}

				void JumpingAIState::update(model::Character &controlled, GameTime &gameTime, GameBase &game)
				{
					if (_machine->getEnemy()->getX() > controlled.getX())
					{
						actions::JumpAction action = actions::JumpAction(&controlled);
						controlled.setDirection(MoveObjDirection::RIGHT);
						game.getEngine()->getInputEngine()->pushAction(action);
						_machine->revertToPreviousState(&gameTime, &game);
					}
					else if (_machine->getEnemy()->getX() < controlled.getX())
					{
						actions::JumpAction action = actions::JumpAction(&controlled);
						controlled.setDirection(MoveObjDirection::LEFT);
						game.getEngine()->getInputEngine()->pushAction(action);
						_machine->revertToPreviousState(&gameTime, &game);
					}
				}

				void JumpingAIState::exit(model::Character &controlled, GameTime &gameTime, GameBase &game) {
					_transition = "";
				}
			}
		}
	}
}