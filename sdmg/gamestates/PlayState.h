//
//
//  @ Project : SDMG
//  @ File Name : PlayState.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "engine\GameState.h"
#include "engine\GameObject.h"
#include "engine\input\Mouse.h"
#include "helperclasses\Editor.h"
#include <vector>
#include <map>

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
		class PlayState : public GameState {
		public:
			virtual void init(GameBase &game);
			virtual void cleanup(GameBase &game);

			virtual void pause(GameBase &game);
			virtual void resume(GameBase &game);

			virtual void handleEvents(GameBase &game, GameTime &gameTime);
			virtual void update(GameBase &game, GameTime &gameTime);
			virtual void draw(GameBase &game, GameTime &gameTime);
			static PlayState& getInstance() {
				static PlayState _instance;
				return _instance;
			}

			void setCharacters(std::vector<model::Character*> *characters);
			void setPlatform(model::Platform *platform);
			void setBullets(std::vector<model::MovablePlatform *> *bullets);
			void setHUDs(std::vector<helperclasses::HUD *> *huds);
			void performDraw(GameBase &game);

			void chooseRandomEnemy();
			void setGameplaySpeed();
			void setPlayer(model::Character *player);
			void setEnemies(std::vector<model::Character*> *enemies);

			int getEnemiesKilled();
			void setEnemiesKilled(int count);
			void setLevel(std::string *level);
		private:
			model::Character *_player;
			std::vector<model::Character*> *_enemies;
			std::string *_level;
		protected:
			std::chrono::high_resolution_clock::time_point _lastUpdate;
			float _step, _accumulator;

			PlayState() { }
			std::vector<helperclasses::HUD*> *_huds;

			bool _showFPS, _showHitBoxes, _showClickBoxes, _canDie;
			long _fps;
			float _multiplier;
			int _enemiesKilled;

			bool _isPaused;

			friend class GameOverState;
			friend class GameOverSurvivalState;

			helperclasses::Editor *_editor;

			GameBase *_game;
		};
	}
}