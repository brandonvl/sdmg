//
//
//  @ Project : SDMG
//  @ File Name : RespawnAction.cpp
//  @ Date : 8-10-2014
//  @ Author : 42IN13SAd
//
//


#include "RespawnAction.h"
#include "model\Character.h"
#include "engine\MovableGameObject.h"
#include "engine\GameBase.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\Engine.h"

namespace sdmg {
	namespace actions {
		RespawnAction::RespawnAction(Character *character) : CharacterAction(character) {}
		RespawnAction::RespawnAction(Character *character, SDL_Event event) : CharacterAction(character, event) {}

		bool RespawnAction::run(engine::GameBase &game) {
			if (_character->stateIsInterruptible())
			{
				if (_event.type == SDL_KEYDOWN) {
					_character->setState(MovableGameObject::State::RESPAWN);
					game.getEngine()->getPhysicsEngine()->doAction(_character, PhysicsEngine::Action::RESPAWN);
				}
				else {
					if (_character->getState() == MovableGameObject::State::RESPAWN)
					{
						_character->setState(MovableGameObject::State::IDLE);
						game.getEngine()->getPhysicsEngine()->doAction(_character, PhysicsEngine::Action::IDLE);
					}
				}
			}
			return true;
		}

		Action* RespawnAction::create(SDL_Event &event) {
			return new RespawnAction(_character, event);
		}
	}
}