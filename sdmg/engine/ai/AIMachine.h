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
					AIMachine(model::Character &controlled, model::Character &enemy) : _controlled(&controlled), _enemy(&enemy), _defaultState(nullptr), _activeState(nullptr), _previousState(nullptr), _concentration(Concentration_SPAN) { };
					virtual ~AIMachine();
					virtual void addState(aistate::AIState *state);
					virtual void setDefaultState(aistate::AIState *state);
					//virtual void setDefaultState(const std::string &stateName);
					virtual void setState(const std::string &stateKey);
					virtual void update(GameTime *gameTime, GameBase *game);
					virtual void revertToPreviousState(GameTime *gameTime, GameBase *game);
					virtual model::Character *getEnemy() { return _enemy; }
					virtual void pause() { _paused = true; }
					virtual void resume() { _paused = false; }
				private:
					void checkState(GameTime *gameTime, GameBase *game);
					std::map<std::string, aistate::AIState*> _states;
					int _concentration;
					const int Concentration_SPAN = 100;
					aistate::AIState *_defaultState = nullptr;
					aistate::AIState *_activeState = nullptr;
					aistate::AIState *_previousState = nullptr;
					model::Character *_controlled = nullptr;
					model::Character *_enemy = nullptr;
					bool _paused = false;
			};
		}
	}
};