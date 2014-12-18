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

				void IdleAIState::enter(model::Character &controlled, GameTime &gameTime, GameBase &game) {

					if (_transition != "")
						_transition = "";
				}

				void IdleAIState::update(model::Character &controlled, GameTime &gameTime, GameBase &game)
				{

					_transition = "moveLeft";
				}

				void IdleAIState::exit(model::Character &controlled, GameTime &gameTime, GameBase &game) {
					_transition = "";
				}
			}
		}
	}
}