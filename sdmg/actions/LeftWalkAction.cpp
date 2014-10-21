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
#include "engine\Engine.h"

namespace sdmg {
	namespace actions {
		LeftWalkAction::LeftWalkAction(Character *character) : CharacterAction(character) {}
		LeftWalkAction::LeftWalkAction(Character *character, SDL_Event event) : CharacterAction(character, event) {}

		bool LeftWalkAction::run(engine::GameBase &game) {
			if (_event.type == SDL_KEYDOWN) {
				_character->setDirection(MovableGameObject::Direction::LEFT);
				if (_character->getState() == MovableGameObject::State::JUMPING || _character->getState() == MovableGameObject::State::JUMPINGLEFT || _character->getState() == MovableGameObject::State::JUMPINGRIGHT)
					_character->setState(MovableGameObject::State::JUMPINGLEFT);
				else if (_character->getState() == MovableGameObject::State::FALLING || _character->getState() == MovableGameObject::State::FALLINGLEFT || _character->getState() == MovableGameObject::State::FALLINGRIGHT)
					_character->setState(MovableGameObject::State::FALLINGLEFT);
				else
					_character->setState(MovableGameObject::State::WALKING);
			}
			else {
				if (_character->getState() == MovableGameObject::State::WALKING && _character->getDirection() == MovableGameObject::Direction::LEFT)
					_character->setState(MovableGameObject::State::IDLE);
				else if (_character->getState() == MovableGameObject::State::JUMPINGLEFT)
					_character->setState(MovableGameObject::State::JUMPING);
				else if (_character->getState() == MovableGameObject::State::FALLINGLEFT)
					_character->setState(MovableGameObject::State::FALLING);

			}
			/*
			if (_event.type == SDL_KEYDOWN) {
				_character->setDirection(MovableGameObject::Direction::LEFT);
				_character->setState(MovableGameObject::State::WALKING);
			}
			else if (_event.type == SDL_KEYUP && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D]) {
					_character->setDirection(MovableGameObject::Direction::RIGHT);
					_character->setState(MovableGameObject::State::WALKING);
			}
			else {
				_character->setState(MovableGameObject::State::IDLE);
			}
			*/
			return true;
		}
		
		Action* LeftWalkAction::create(SDL_Event &event) {
			return new LeftWalkAction(_character, event);
		}
		
	}
}
