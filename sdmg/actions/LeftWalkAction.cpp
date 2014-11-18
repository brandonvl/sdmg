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
			if (_character->stateIsInterruptible())
			{
				if (_event.type == SDL_KEYDOWN) {
					_character->setDirection(MovableGameObject::Direction::LEFT);

					if (_character->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACKBEGIN))
						_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACKBEGIN);
					else if (_character->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACK))
						_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACK);
					else if (_character->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACKEND))
						_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACKEND);

					else if (_character->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACKBEGIN))
						_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACKBEGIN);
					else if (_character->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACK))
						_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACK);
					else if (_character->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACKEND))
						_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACKEND);

					else if (_character->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::MIDRANGEATTACKBEGIN))
						_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACKBEGIN);
					else if (_character->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::MIDRANGEATTACK))
						_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACK);
					else if (_character->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::MIDRANGEATTACKEND))
						_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACKEND);

					else if (_character->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::LONGRANGEATTACKBEGIN))
						_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACKBEGIN);
					else if (_character->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::LONGRANGEATTACK))
						_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACK);
					else if (_character->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::LONGRANGEATTACKEND))
						_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACKEND);


					else if (_character->getState() == MovableGameObject::State::JUMPING
						|| _character->getState() == MovableGameObject::State::JUMPINGLEFT
						|| _character->getState() == MovableGameObject::State::JUMPINGRIGHT)
						_character->setState(MovableGameObject::State::JUMPINGLEFT);
					else if (_character->getState() == MovableGameObject::State::FALLING
						|| _character->getState() == MovableGameObject::State::FALLINGLEFT
						|| _character->getState() == MovableGameObject::State::FALLINGRIGHT)
						_character->setState(MovableGameObject::State::FALLINGLEFT);
					else
						_character->setState(MovableGameObject::State::WALKING);
				}
				// KeyUP
				else {
					if (_character->getDirection() == MovableGameObject::Direction::LEFT)
					{
						if (_character->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::FORWARD_ROLL)
							|| _character->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::FORWARD_ROLL))
							_character->setState(MovableGameObject::State::IDLE | MovableGameObject::State::FORWARD_ROLL);

						else if (_character->getState() == (MovableGameObject::State::JUMPING | MovableGameObject::State::FORWARD_ROLL))
							_character->setState(MovableGameObject::State::JUMPING | MovableGameObject::State::FORWARD_ROLL);
						else if (_character->getState() == (MovableGameObject::State::JUMPINGLEFT | MovableGameObject::State::FORWARD_ROLL))
							_character->setState(MovableGameObject::State::JUMPING | MovableGameObject::State::FORWARD_ROLL);
						else if (_character->getState() == (MovableGameObject::State::JUMPINGRIGHT | MovableGameObject::State::FORWARD_ROLL))
						{
							_character->setDirection(MovableGameObject::Direction::RIGHT);
							_character->setState(MovableGameObject::State::JUMPING | MovableGameObject::State::FORWARD_ROLL);
						}
						else if (_character->getState() == (MovableGameObject::State::FALLING | MovableGameObject::State::FORWARD_ROLL))
							_character->setState(MovableGameObject::State::FALLING | MovableGameObject::State::FORWARD_ROLL);
						else if (_character->getState() == (MovableGameObject::State::FALLINGLEFT | MovableGameObject::State::FORWARD_ROLL))
							_character->setState(MovableGameObject::State::FALLING | MovableGameObject::State::FORWARD_ROLL);
						else if (_character->getState() == (MovableGameObject::State::FALLINGRIGHT | MovableGameObject::State::FORWARD_ROLL))
						{
							_character->setDirection(MovableGameObject::Direction::RIGHT);
							_character->setState(MovableGameObject::State::FALLING | MovableGameObject::State::FORWARD_ROLL);
						}

						else if (_character->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACKBEGIN))
							_character->setState(MovableGameObject::State::IDLE | MovableGameObject::State::MIDRANGEATTACKBEGIN);
						else if (_character->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACK))
							_character->setState((MovableGameObject::State::IDLE | MovableGameObject::State::MIDRANGEATTACK));
						else if (_character->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACKEND))
							_character->setState((MovableGameObject::State::IDLE | MovableGameObject::State::MIDRANGEATTACKEND));

						else if (_character->getState() == MovableGameObject::State::WALKING)
							_character->setState(MovableGameObject::State::IDLE);
						else if (_character->getState() == MovableGameObject::State::JUMPINGLEFT)
							_character->setState(MovableGameObject::State::JUMPING);
						else if (_character->getState() == MovableGameObject::State::FALLINGLEFT || _character->getState() == MovableGameObject::State::FALLINGRIGHT)
							_character->setState(MovableGameObject::State::FALLING);
					}
				}
			}
			else
			{
				if (_event.type == SDL_KEYDOWN)
				{
					if (_character->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::FORWARD_ROLL)
						|| _character->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::FORWARD_ROLL))
					{
						if (_character->getDirection() == MovableGameObject::Direction::LEFT)
							_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::FORWARD_ROLL);
						else if (_character->getDirection() == MovableGameObject::Direction::RIGHT)
							_character->setShouldTurnArround(true);
					}

					else if (_character->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::MIDRANGEATTACKBEGIN))
					{
						_character->setDirection(MovableGameObject::Direction::LEFT);
						_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACKBEGIN);
					}
					else if (_character->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::MIDRANGEATTACK))
					{
						_character->setDirection(MovableGameObject::Direction::LEFT);
						_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACK);
					}
					else if (_character->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::MIDRANGEATTACKEND))
					{
						_character->setDirection(MovableGameObject::Direction::LEFT);
						_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACKEND);
					}

					else if (_character->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::LONGRANGEATTACKBEGIN))
					{
						_character->setDirection(MovableGameObject::Direction::LEFT);
						_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACKBEGIN);
					}
					else if (_character->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::LONGRANGEATTACK))
					{
						_character->setDirection(MovableGameObject::Direction::LEFT);
						_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACK);
					}
					else if (_character->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::LONGRANGEATTACKEND))
					{
						_character->setDirection(MovableGameObject::Direction::LEFT);
						_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACKEND);
					}

					else if (_character->getState() == (MovableGameObject::State::JUMPING | MovableGameObject::State::FORWARD_ROLL))
					{
						_character->setState(MovableGameObject::State::JUMPINGLEFT | MovableGameObject::State::FORWARD_ROLL);
					}
					else if (_character->getState() == (MovableGameObject::State::JUMPINGLEFT | MovableGameObject::State::FORWARD_ROLL))
					{
						_character->setState(MovableGameObject::State::JUMPINGLEFT | MovableGameObject::State::FORWARD_ROLL);
					}
					else if (_character->getState() == (MovableGameObject::State::JUMPINGRIGHT | MovableGameObject::State::FORWARD_ROLL))
					{
						_character->setShouldTurnArround(true);
						_character->setState(MovableGameObject::State::JUMPINGLEFT | MovableGameObject::State::FORWARD_ROLL);
					}

					else if (_character->getState() == (MovableGameObject::State::FALLING | MovableGameObject::State::FORWARD_ROLL))
					{
						_character->setState(MovableGameObject::State::FALLINGLEFT | MovableGameObject::State::FORWARD_ROLL);
					}
					else if (_character->getState() == (MovableGameObject::State::FALLINGLEFT | MovableGameObject::State::FORWARD_ROLL))
					{
						_character->setState(MovableGameObject::State::FALLINGLEFT | MovableGameObject::State::FORWARD_ROLL);
					}
					else if (_character->getState() == (MovableGameObject::State::FALLINGRIGHT | MovableGameObject::State::FORWARD_ROLL))
					{
						_character->setShouldTurnArround(true);
						_character->setState(MovableGameObject::State::FALLINGLEFT | MovableGameObject::State::FORWARD_ROLL);
					}

				}
				else
				{
					if (_character->getState() == (MovableGameObject::State::IDLE | MovableGameObject::State::FORWARD_ROLL)
						|| _character->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::FORWARD_ROLL))
					{
						if (!_character->getShouldTurnArround())
							_character->setState(MovableGameObject::State::IDLE | MovableGameObject::State::FORWARD_ROLL);
						else if (_character->getDirection() == MovableGameObject::Direction::RIGHT)
							_character->setState(MovableGameObject::State::WALKING | MovableGameObject::State::FORWARD_ROLL);
					}
					else if (_character->getState() == (MovableGameObject::State::JUMPINGLEFT | MovableGameObject::State::FORWARD_ROLL))
					{
						if (!_character->getShouldTurnArround())
							_character->setState(MovableGameObject::State::JUMPING | MovableGameObject::State::FORWARD_ROLL);
					}
					else if (_character->getState() == (MovableGameObject::State::JUMPINGRIGHT | MovableGameObject::State::FORWARD_ROLL))
					{
						_character->setState(MovableGameObject::State::JUMPINGRIGHT | MovableGameObject::State::FORWARD_ROLL);
					}
					else if (_character->getState() == (MovableGameObject::State::FALLINGLEFT | MovableGameObject::State::FORWARD_ROLL))
					{
						if (!_character->getShouldTurnArround())
							_character->setState(MovableGameObject::State::FALLING | MovableGameObject::State::FORWARD_ROLL);
					}
					else if (_character->getState() == (MovableGameObject::State::FALLINGRIGHT | MovableGameObject::State::FORWARD_ROLL))
					{
						_character->setState(MovableGameObject::State::FALLINGRIGHT | MovableGameObject::State::FORWARD_ROLL);
					}

					else if (_character->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACKBEGIN))
						_character->setState(MovableGameObject::State::IDLE | MovableGameObject::State::MIDRANGEATTACKBEGIN);
					else if (_character->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACK))
						_character->setState((MovableGameObject::State::IDLE | MovableGameObject::State::MIDRANGEATTACK));
					else if (_character->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACKEND))
						_character->setState((MovableGameObject::State::IDLE | MovableGameObject::State::MIDRANGEATTACKEND));

					else if (_character->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACKBEGIN))
						_character->setState(MovableGameObject::State::IDLE | MovableGameObject::State::LONGRANGEATTACKBEGIN);
					else if (_character->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACK))
						_character->setState((MovableGameObject::State::IDLE | MovableGameObject::State::LONGRANGEATTACK));
					else if (_character->getState() == (MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACKEND))
						_character->setState((MovableGameObject::State::IDLE | MovableGameObject::State::LONGRANGEATTACKEND));
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
		
		Action* LeftWalkAction::create(SDL_Event &event) {
			return new LeftWalkAction(_character, event);
		}
		
	}
}
