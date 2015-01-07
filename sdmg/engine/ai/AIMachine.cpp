#include "AIMachine.h"
#include "aistates\AIState.h"
#include "model\Character.h"
#include "engine\GameBase.h"
#include "engine\GameTime.h"

namespace sdmg {
	namespace engine {
		namespace ai {
			AIMachine::~AIMachine() {

				for (auto &it : _states) {
					delete it.second;
				}

			}

			void AIMachine::checkState(GameTime *gameTime, GameBase *game) {

				if (_activeState != nullptr) {
					if (_activeState->isComplete()) {
						setState(_activeState->getTransition());
						_previousState->exit(*_controlled, *gameTime, *game);
						_activeState->enter(*_controlled, *gameTime, *game);
					}
				}
				else {
					if (_defaultState != nullptr) {
						_activeState = _defaultState;
					}
				}

			}

			void AIMachine::addState(aistate::AIState *state) {
				_states.insert(std::make_pair(state->getStateKey(), state));
			}

			void AIMachine::setDefaultState(aistate::AIState *state) {
				addState(state);
				_defaultState = state;

				if (_activeState == nullptr)
					_activeState = state;
			}

			/*void AIMachine::setDefaultState(const std::string &stateName) {
				auto it = _states.find(stateName);

				if (it != _states.end()) {
					_defaultState = it->second;
				}
			}*/

			void AIMachine::revertToPreviousState(GameTime *gameTime, GameBase *game) {
				
				_activeState->exit(*_controlled, *gameTime, *game);

				_activeState = _previousState ? _previousState : _defaultState;

				if (_activeState)
					_activeState->enter(*_controlled, *gameTime, *game);
			}

			void AIMachine::setState(const std::string &stateKey) {

				auto state = _states.find(stateKey);

				if (state->second != nullptr) {
					_previousState = _activeState;
					_activeState = _states[stateKey];
				}
			}

			void AIMachine::update(GameTime *gameTime, GameBase *game) {

				if (!_paused){

					checkState(gameTime, game);

					if (_activeState != nullptr) {
						_activeState->update(*_controlled, *gameTime, *game);
					}
				}
			}
		}
	}
}