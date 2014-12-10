#include "IdleAIState.h"
#include "engine\ai\AIMachine.h"
#include "model\Character.h"
#include "engine\MovableGameObject.h"
#include "engine\GameBase.h"
#include "engine\GameTime.h"
#include "helperclasses\RandomGenerator.h"
#include "engine\World.h"
#include "engine\Engine.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\physics\RayCastCallBack.h"
#include <random>
//#include <functional>

#include <iostream>

namespace sdmg {
	namespace engine {
		namespace ai {
			namespace aistate {

				bool _objFound = false;
				MovableGameObject *_enemy;

				float32 IdleAIState::testBack(b2Body *body) {
					std::cout << "called" << std::endl;
					//if (body)

					return -1.0F;
				}

				void IdleAIState::update(model::Character &controlled, GameTime &gameTime, GameBase &game)
				{
					
					if (!_enemy)
						_enemy = game.getWorld()->getPlayers()[1];

					_machine->setState("moveLeft");
						/*
						if (controlled.getState() != (MoveObjState::LONGRANGEATTACKBEGIN | MoveObjState::WALKING) && controlled.stateIsInterruptible())
							controlled.setState(MoveObjState::LONGRANGEATTACKBEGIN | MoveObjState::WALKING);
						else if (controlled.stateIsInterruptible())
							_machine->setState("moveRight");*/
					/*if (enemy->getX() > controlled.getX())
						_machine->setState("moveRight");
					else if (enemy->getX() < controlled.getX())
						_machine->setState("moveLeft");*/
				}
			}
		}
	}
}