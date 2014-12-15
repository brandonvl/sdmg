#pragma once

#include <map>
#include <string>

namespace sdmg {

	namespace model {
		class Character;
	}

	namespace engine {
		class GameTime;
		class GameBase;
		namespace ai {

			namespace aistate {
				class AIState;
			}

			class AIMachine
			{
				public:
					AIMachine(model::Character &controlled) : _controlled(&controlled), _defaultState(nullptr), _activeState(nullptr), _previousState(nullptr) { };
					virtual ~AIMachine();
					virtual void addState(aistate::AIState *state);
					virtual void setDefaultState(aistate::AIState *state);
					//virtual void setDefaultState(const std::string &stateName);
					virtual void setState(const std::string &stateKey);
					virtual void update(GameTime *gameTime, GameBase *game);
					virtual void revertToPreviousState();
				private:
					void checkState(GameTime *gameTime, GameBase *game);
					std::map<std::string, aistate::AIState*> _states;
					aistate::AIState *_defaultState;
					aistate::AIState *_activeState;
					aistate::AIState *_previousState;
					model::Character *_controlled;
			};
		}
	}
};