//
//
//  @ Project : SDMG
//  @ File Name : LoadingSurvivalState.h
//  @ Date : 10-12-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "engine\GameState.h"
#include <string>

using namespace sdmg::engine;

namespace JSON { class JSONArray; }
namespace sdmg {
	namespace model {
		class Platform;
		class Character;
		class MovablePlatform;
	}
	namespace helperclasses{ class HUD; }
	namespace gamestates {
		class LoadingSurvivalState : public GameState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void pause(GameBase &game);
			void resume(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);
			static LoadingSurvivalState& getInstance() {
				static LoadingSurvivalState _instance;
				return _instance;
			}

			std::string getLevelName();
			std::string getPlayerName();
			void setPlayerName(std::string playerName);
			
		protected:
			LoadingSurvivalState() { }
		private:
			GameBase *_game;
			void load();
			void loadCharacters(JSON::JSONArray &startingPositions);
			void loadBulletBobs(JSON::JSONArray &bobs);
			void loadLevel();
			void loadKeybindings();
			void clearEventQueue();
			void setEnemies();
			bool _isLoaded, _isError, _isTutorial;
			std::vector<helperclasses::HUD*> *_huds;

			std::string *_playerName, *_levelName;
			std::vector<std::string*> *_enemyNames;
			std::vector<model::Character*> *_enemies;

			// LoadingBar
			int _loadingValue, _loadingStep, _marginInner, _marginValue, _totalWidth, _totalHeight, _loadingBarX, _loadingBarY;

			bool _isAdvertisement;
			int _advertisementX, _advertisementY;
			void loadAdvertisement();
		};
	}
}
