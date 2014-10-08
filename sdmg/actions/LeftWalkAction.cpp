//
//
//  @ Project : SDMG
//  @ File Name : LeftWalkAction.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "LeftWalkAction.h"
#include "model\Character.h"
#include "engine\MovableGameObject.h"
#include "engine\GameBase.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\Engine.h"

namespace sdmg {
	namespace actions {
		LeftWalkAction::LeftWalkAction(Character *character) : CharacterAction(character) {}
		LeftWalkAction::LeftWalkAction(Character *character, SDL_Event event) : CharacterAction(character, event) {}

		bool LeftWalkAction::run(engine::GameBase &game) {
			if (_event.type == SDL_KEYDOWN) {
				_character->setDirection(MovableGameObject::Direction::LEFT);
				_character->setState(MovableGameObject::State::WALKING);
				game.getEngine()->getPhysicsEngine()->registerAction(_character);
			}
			else {
				_character->setState(MovableGameObject::State::IDLE);
			}
			return true;
		}
		
		Action* LeftWalkAction::create(SDL_Event &event) {
			return new LeftWalkAction(_character, event);
		}
		
	}
}
