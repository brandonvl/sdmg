//
//
//  @ Project : SDMG
//  @ File Name : RollAction.cpp
//  @ Date : 4-10-2014
//  @ Author : 42IN13SAd
//
//


#include "RollAction.h"
#include "model\Character.h"
#include "engine\MovableGameObject.h"
#include "engine\GameBase.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\Engine.h"

namespace sdmg {
	namespace actions {
		RollAction::RollAction(Character *character) : CharacterAction(character) {}
		RollAction::RollAction(Character *character, SDL_Event event) : CharacterAction(character, event) {}

		bool RollAction::run(engine::GameBase &game) {
			if (_character->stateIsInterruptible())
			{
				if (_event.type == SDL_KEYDOWN) {
					_character->setState(MovableGameObject::State::FORWARD_ROLL);
					game.getEngine()->getPhysicsEngine()->doAction(_character, _character->getDirection() == MovableGameObject::Direction::RIGHT ? PhysicsEngine::Action::MOVERIGHT : PhysicsEngine::Action::MOVELEFT);
				}
			}
			return true;
		}

		Action* RollAction::create(SDL_Event &event) {
			return new RollAction(_character, event);
		}
	}
}