#include "IdleAIState.h"
#include "engine\ai\AIMachine.h"
#include "model\Character.h"
#include "engine\MovableGameObject.h"
#include "engine\GameBase.h"
#include "engine\GameTime.h"
#include "helperclasses\RandomGenerator.h"
#include "engine\World.h"
#include <random>

#include <iostream>

namespace sdmg {
	namespace engine {
		namespace ai {
			namespace aistate {


				void IdleAIState::update(model::Character &controlled, GameTime &gameTime, GameBase &game)
				{
					MovableGameObject *enemy = game.getWorld()->getPlayers()[1];

					if (enemy->getX() > controlled.getX())
						_machine->setState("moveRight");
					else if (enemy->getX() < controlled.getX())
						_machine->setState("moveLeft");
				}
			}
		}
	}
}