//
//
//  @ Project : SDMG
//  @ File Name : JumpAction.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "JumpAction.h"
#include "model\Character.h"
#include "engine\MovableGameObject.h"
#include "engine\GameBase.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\Engine.h"

namespace sdmg {
	namespace actions {
		JumpAction::JumpAction(Character *character) : CharacterAction(character) {}
		JumpAction::JumpAction(Character *character, SDL_Event event) : CharacterAction(character, event) {}

		bool JumpAction::run(engine::GameBase &game) {
			if (_event.type == SDL_KEYDOWN) {
				if (!_character->getIsJumping())
				{
					if (_character->getState() != MovableGameObject::State::JUMPING)
					{
						_character->setState(MovableGameObject::State::JUMPING);
						_character->setIsJumping(true);
						game.getEngine()->getPhysicsEngine()->doAction(_character, PhysicsEngine::Action::JUMP);
					}
				}
			}
			else {
				if (_character->getState() == MovableGameObject::State::JUMPING)
				{
					_character->setState(MovableGameObject::State::IDLE);
					game.getEngine()->getPhysicsEngine()->doAction(_character, PhysicsEngine::Action::IDLE);
				}
			}
			return true;
		}
		
		Action* JumpAction::create(SDL_Event &event) {
			return new JumpAction(_character, event);
		}
		
	}
}
