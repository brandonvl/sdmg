//
//
//  @ Project : SDMG
//  @ File Name : RightWalkAction.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "RightWalkAction.h"
#include "model\Character.h"
#include "engine\MovableGameObject.h"
#include "engine\GameBase.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\Engine.h"

namespace sdmg {
	namespace actions {
		RightWalkAction::RightWalkAction(Character *character) : CharacterAction(character) {}
		RightWalkAction::RightWalkAction(Character *character, SDL_Event event) : CharacterAction(character, event) {}

		bool RightWalkAction::run(engine::GameBase &game) {
			if (_event.type == SDL_KEYDOWN) {
				_character->setDirection(MovableGameObject::Direction::RIGHT);
				_character->setState(MovableGameObject::State::WALKING);
				game.getEngine()->getPhysicsEngine()->registerAction(_character);
			}
			else {
				_character->setState(MovableGameObject::State::IDLE);
			}
			return true;
		}
		
		Action* RightWalkAction::create(SDL_Event &event) {
			return new RightWalkAction(_character, event);
		}
		
	}
}
