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
		RollAction::RollAction(Character *character) : CharacterAction(character, "RollAction") {}
		RollAction::RollAction(Character *character, SDL_Event event) : CharacterAction(character, event, "RollAction") {}

		bool RollAction::run(engine::GameBase &game) {
			CharacterAction::run(game);

			if (_character->stateIsInterruptible())
			{
				if (_character->getPP() >= 10)
				{
					if (_event.type == SDL_KEYDOWN) {
						if (_character->getState() == MovableGameObject::State::WALKING)
							_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::FORWARD_ROLL);
						else if (_character->getState() == MovableGameObject::State::JUMPING)
							_character->setState(MovableGameObject::State::JUMPING | MovableGameObject::State::FORWARD_ROLL);
						else if (_character->getState() == MovableGameObject::State::JUMPINGLEFT)
							_character->setState(MovableGameObject::State::JUMPINGLEFT | MovableGameObject::State::FORWARD_ROLL);
						else if (_character->getState() == MovableGameObject::State::JUMPINGRIGHT)
							_character->setState(MovableGameObject::State::JUMPINGRIGHT | MovableGameObject::State::FORWARD_ROLL);


						else if (_character->getState() == MovableGameObject::State::FALLING)
							_character->setState(MovableGameObject::State::FALLING | MovableGameObject::State::FORWARD_ROLL);
						else if (_character->getState() == MovableGameObject::State::FALLINGLEFT)
							_character->setState(MovableGameObject::State::FALLINGLEFT | MovableGameObject::State::FORWARD_ROLL);
						else if (_character->getState() == MovableGameObject::State::FALLINGRIGHT)
							_character->setState(MovableGameObject::State::FALLINGRIGHT | MovableGameObject::State::FORWARD_ROLL);


						else
							_character->setState(MovableGameObject::State::IDLE | MovableGameObject::State::FORWARD_ROLL);

						_character->addPP(-10);
						//  game.getEngine()->getPhysicsEngine()->doAction(_character, _character->getDirection() == MovableGameObject::Direction::RIGHT ? PhysicsEngine::Action::MOVERIGHT : PhysicsEngine::Action::MOVELEFT);
					}
				}
			}
			return true;
		}

		Action* RollAction::create(SDL_Event &event) {
			return new RollAction(_character, event);
		}
	}
}