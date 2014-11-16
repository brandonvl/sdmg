//
//
//  @ Project : SDMG
//  @ File Name : BlockAction.cpp
//  @ Date : 11-11-2014
//  @ Author : 42IN13SAd
//
//


#include "BlockAction.h"
#include "model\Character.h"
#include "engine\MovableGameObject.h"
#include "engine\GameBase.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\Engine.h"

namespace sdmg {
	namespace actions {
		BlockAction::BlockAction(Character *character) : CharacterAction(character) {}
		BlockAction::BlockAction(Character *character, SDL_Event event) : CharacterAction(character, event) {}

		bool BlockAction::run(engine::GameBase &game) {
			if (_character->stateIsInterruptible())
			{
				if (_event.type == SDL_KEYDOWN) {
					if (_character->getBP() > 0)
						_character->setState(MovableGameObject::State::IDLE | MovableGameObject::State::BLOCKING);
				}
				else {
					if (_character->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::BLOCKING))
					_character->setState(MovableGameObject::State::IDLE);
				}
			}
			return true;
		}

		Action* BlockAction::create(SDL_Event &event) {
			return new BlockAction(_character, event);
		}
	}
}
