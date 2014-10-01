//
//
//  @ Project : SDMG
//  @ File Name : GameStateManager.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once

#include "GameStateManager.h"
#include "GameState.h"

namespace sdmg {
	namespace engine {
		GameStateManager::GameStateManager(GameBase *engine)
		{
			_engine = engine;
			_states = std::vector<GameState*>();
		}

		GameStateManager::~GameStateManager()
		{

		}

		void GameStateManager::cleanup()
		{
			// cleanup the all states
			if (!_states.empty()) {
				for (auto item : _states)
				{
					delete item;
				}
			}
		}

		void GameStateManager::changeState(GameState *state)
		{
			// cleanup the current state
			if (!_states.empty()) {
				_states.back()->cleanup();
				_states.pop_back();
			}

			// store and init the new state
			_states.push_back(state);
			_states.back()->init();
		}

		void GameStateManager::pushState(GameState *state)
		{
			// pause current state
			if (!_states.empty()) {
				_states.back()->pause();
			}

			// store and init the new state
			_states.push_back(state);
			_states.back()->init();
		}

		void GameStateManager::popState()
		{
			// cleanup the current state
			if (!_states.empty()) {
				_states.back()->cleanup();
				_states.pop_back();
			}

			// resume previous state
			if (!_states.empty()) {
				_states.back()->resume();
			}
		}

		void GameStateManager::handleEvents()
		{
			// let the state handle events
			_states.back()->handleEvents(_engine,_engine->_gameTime);
		}

		void GameStateManager::update()
		{
			// let the state update the game
			_states.back()->update(_engine, _engine->_gameTime);
		}

		void GameStateManager::draw()
		{
			// let the state draw the screen
			_states.back()->draw(_engine, _engine->_gameTime);
		}
	}
}
