//
//
//  @ Project : SDMG
//  @ File Name : PlayBackState.h
//  @ Date : 18-12-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "engine\GameState.h"
#include "engine\GameObject.h"
#include "engine\input\Mouse.h"
#include "engine\input\Action.h"
#include "actions\Actions.h"
#include <vector>
#include <map>
#include <queue>
#include <string>

using namespace sdmg::engine;

namespace sdmg {
	namespace model {
		class Platform;
		class Character;
		class MovablePlatform;
	}
	namespace helperclasses{
		class HUD;
	}
	namespace gamestates {
		class PlayBackState : public GameState {
		public:
			virtual void init(GameBase &game);
			virtual void cleanup(GameBase &game);

			virtual void pause(GameBase &game);
			virtual void resume(GameBase &game);

			virtual void handleEvents(GameBase &game, GameTime &gameTime);
			virtual void update(GameBase &game, GameTime &gameTime);
			virtual void draw(GameBase &game, GameTime &gameTime);
			static PlayBackState& getInstance() {
				static PlayBackState _instance;
				return _instance;
			}

			void setCharacters(std::vector<model::Character*> *characters);
			void setPlatform(model::Platform *platform);
			void setBullets(std::vector<model::MovablePlatform *> *bullets);
			void setHUDs(std::vector<helperclasses::HUD *> *huds);
			void performDraw(GameBase &game);

			void loadPlayback(std::string name);

			struct RecordStep
			{
				std::string _action;
				int _characterID;
				long long _timestamp;
				bool _keyDown;

				RecordStep(std::string action, int characterID, long long timestamp, bool keyDown) : _action(action), _characterID(characterID), _timestamp(timestamp), _keyDown(keyDown){};
			};

			void setPlaybackSteps(std::queue<RecordStep*> *recordQueue);
			void addAction(std::string name, Action *action);

		private:
			std::chrono::high_resolution_clock::time_point _lastUpdate;
			std::chrono::high_resolution_clock::time_point _timeStart;
			float _step, _accumulator, _multiplier;

			PlayBackState() { }
			std::vector<helperclasses::HUD*> *_huds;
			
			std::queue<RecordStep*> *_recordQueue;
			std::map<std::string, Action*> *_recordMap;

			bool _particlesSet, _isPaused;

			GameBase *_game;
		};
	}
}