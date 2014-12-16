#pragma once

#include "engine\ai\EasyAIMachine.h"
#include "model\Character.h"
#include "aistates\IdleAIState.h"
#include "aistates\MoveLeftAIState.h"
#include "aistates\MoveRightAIState.h"


namespace sdmg {

	namespace engine {
		namespace ai {
			EasyAIMachine::EasyAIMachine(model::Character &controlled, model::Character &enemy) : AIMachine(controlled, enemy)
			{
				setDefaultState(new aistate::IdleAIState(*this));
				addState(new aistate::MoveLeftAIState(*this));
				addState(new aistate::MoveRightAIState(*this));
			}
		}
	}
}