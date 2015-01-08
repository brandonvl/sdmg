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
#include <unordered_map>
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

			//void loadPlayback(std::string name);

			struct RecordStep
			{
				Action *_action;
				int _timestamp, _hp, _lives, _pp;
				float _x, _y, _velocityX, _velocityY;
				Character *_character;

				RecordStep(Action *action, int timestamp, float x, float y, float velocityX, float velocityY, Character *character, int hp, int lives, int pp) : _action(action), _timestamp(timestamp), _x(x), _y(y), _velocityX(velocityX), _velocityY(velocityY), _character(character), _hp(hp), _lives(lives), _pp(pp){
					
				};
			};

			void setPlaybackSteps(std::queue<RecordStep*> *recordQueue);

		private:
			std::chrono::high_resolution_clock::time_point _lastUpdate;
			std::chrono::high_resolution_clock::time_point _timeStart;
			float _step, _accumulator, _multiplier;

			PlayBackState() {
				_recordQueue = new std::queue<RecordStep*>();
			}

			std::vector<helperclasses::HUD*> *_huds;
			
			std::queue<RecordStep*> *_recordQueue;

			friend class GameOverState;

			bool _particlesSet, _isPaused, _threadSpawned, _running = true;

			GameBase *_game;

			friend class LoadingPlayBackState;

			void actionThread();
		};
	}
}