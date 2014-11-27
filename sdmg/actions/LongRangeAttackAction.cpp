//
//
//  @ Project : SDMG
//  @ File Name : MidRangeAttackAction.cpp
//  @ Date : 12-11-2014
//  @ Author : 42IN13SAd
//
//

#include "LongRangeAttackAction.h"
#include "model\Character.h"
#include "engine\MovableGameObject.h"
#include "engine\GameBase.h"
#include "engine\Engine.h"

namespace sdmg {
	namespace actions {
		LongRangeAttackAction::LongRangeAttackAction(Character *character) : CharacterAction(character) {}
		LongRangeAttackAction::LongRangeAttackAction(Character *character, SDL_Event event) : CharacterAction(character, event) {}

		bool LongRangeAttackAction::run(engine::GameBase &game) {
			if (_character->stateIsInterruptible() && _character->getShootBody() == nullptr)
			{
				if (_event.type == SDL_KEYDOWN) {
					if (_character->getPP() >= 20)
					{
						if (_character->getState() != (MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACKBEGIN) &&
							_character->getState() != (MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACK) &&
							_character->getState() != (MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACKEND))
						{
							if (_character->getState() == MovableGameObject::State::WALKING)
								_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACKBEGIN);
							else
								_character->setState(MovableGameObject::State::IDLE | MovableGameObject::State::LONGRANGEATTACKBEGIN);
						}
					}
				}
			}
			return true;
		}

		Action* LongRangeAttackAction::create(SDL_Event &event) {
			return new LongRangeAttackAction(_character, event);
		}
	}
}