#include "AIMachine.h"
#include "aistates\AIState.h"
#include "model\Character.h"
#include "engine\GameBase.h"
#include "engine\GameTime.h"
#include "engine\World.h"


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

					MovableGameObject *newEnemy = nullptr;
					if (_concentration >= Concentration_SPAN)
					{
						_concentration = 0;
						float smallestDistance = 0.0f;
						auto players = game->getWorld()->getPlayers();
						for (size_t i = 0; i < players.size(); i++)
						{
							if (players[i] != _controlled && players[i]->getLives() > 0)
							{
								float distance = std::abs(sqrt(pow(_controlled->getPixelX() - _enemy->getPixelX(), 2) + pow(_controlled->getPixelY() - _enemy->getPixelY(), 2)));
								if (distance < smallestDistance)
								{
									smallestDistance = distance;
									newEnemy = players[i];
								}
							}
						}
					}
					else _concentration++;
					if (newEnemy)
						_enemy = static_cast<model::Character*>(newEnemy);

					checkState(gameTime, game);

					if (_activeState != nullptr) {
						_activeState->update(*_controlled, *gameTime, *game);
					}
				}
			}
		}
	}
}