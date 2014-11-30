#include "AIMachine.h"
#include "aistates\AIState.h"
#include "model\Character.h"
#include "engine\GameBase.h"
#include "engine\GameTime.h"

namespace sdmg {
	namespace engine {
		namespace ai {
			AIMachine::~AIMachine() {

			}

			void AIMachine::checkState() {

				if (_activeState != nullptr) {

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

			void AIMachine::setState(const std::string &stateKey) {
				_activeState = _states[stateKey];
			}

			void AIMachine::update(GameTime *gameTime, GameBase *game) {
				checkState();

				if (_activeState != nullptr) {
					_activeState->update(*_controlled, *gameTime, *game);
				}
			}
		}
	}
}