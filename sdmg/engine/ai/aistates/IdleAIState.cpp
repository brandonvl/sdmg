#include "IdleAIState.h"
#include "engine\ai\AIMachine.h"
#include "model\Character.h"
#include "engine\GameBase.h"
#include "engine\GameTime.h"

#include <iostream>

namespace sdmg {
	namespace engine {
		namespace ai {
			namespace aistate {

				const double time = 1000;
				double updater = 0;

				void IdleAIState::update(model::Character &controlled, GameTime &gameTime, GameBase &game)
				{
					if (updater >= time)
						_machine->setState("moveRight");
					else
					{
						updater++;
					}
				}
			}
		}
	}
}