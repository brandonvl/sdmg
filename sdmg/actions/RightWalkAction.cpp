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
			if (_character->stateIsInterruptible())
			{
				if (_event.type == SDL_KEYDOWN) {
					_character->setDirection(MovableGameObject::Direction::RIGHT);
					if (_character->getState() == MovableGameObject::State::JUMPING || _character->getState() == MovableGameObject::State::JUMPINGLEFT || _character->getState() == MovableGameObject::State::JUMPINGRIGHT)
						_character->setState(MovableGameObject::State::JUMPINGRIGHT);
					else if (_character->getState() == MovableGameObject::State::FALLING || _character->getState() == MovableGameObject::State::FALLINGLEFT || _character->getState() == MovableGameObject::State::FALLINGRIGHT)
						_character->setState(MovableGameObject::State::FALLINGRIGHT);
					else
						_character->setState(MovableGameObject::State::WALKING);
				}
				else {
					if (_character->getState() == MovableGameObject::State::WALKING && _character->getDirection() == MovableGameObject::Direction::RIGHT)
						_character->setState(MovableGameObject::State::IDLE);
					else if (_character->getState() == MovableGameObject::State::JUMPINGRIGHT)
						_character->setState(MovableGameObject::State::JUMPING);
					else if (_character->getState() == MovableGameObject::State::FALLINGRIGHT)
						_character->setState(MovableGameObject::State::FALLING);
				}
			}
			/*
			if (_event.type == SDL_KEYDOWN) {
				_character->setDirection(MovableGameObject::Direction::RIGHT);
				_character->setState(MovableGameObject::State::WALKING);
			}
			else if (_event.type == SDL_KEYUP && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A]) {
				_character->setDirection(MovableGameObject::Direction::LEFT);
				_character->setState(MovableGameObject::State::WALKING);
			}
			else {
				_character->setState(MovableGameObject::State::IDLE);
			}
			*/
			return true;
		}
		
		Action* RightWalkAction::create(SDL_Event &event) {
			return new RightWalkAction(_character, event);
		}
		
	}
}
