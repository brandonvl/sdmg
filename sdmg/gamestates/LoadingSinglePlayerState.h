//
//
//  @ Project : SDMG
//  @ File Name : LoadingSinglePlayerState.h
//  @ Date : 8-12-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "engine\GameState.h"
#include <string>
struct SDL_Thread;

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
		class LoadingSinglePlayerState : public GameState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void pause(GameBase &game);
			void resume(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);
			static LoadingSinglePlayerState& getInstance() {
				static LoadingSinglePlayerState _instance;
				return _instance;
			}

			void unloadAll();
			std::string getPlayerName();
			void setPlayerName(std::string playerName);
			void loadNextFight();
			bool hasFinished();

		protected:
			LoadingSinglePlayerState() { }
		private:
			GameBase *_game;
			void load();
			void loadCharacters();
			void loadBulletBobs(JSON::JSONArray &bobs);
			void loadLevel(std::string level);
			void loadKeybindings();
			void clearEventQueue();
			void setEnemies();
			bool _isLoaded, _isError;
			std::vector<helperclasses::HUD*> *_huds;
			
			std::string *_playerName;
			std::vector<std::string*> *_enemies;
			
			int _advertisementX, _advertisementY;
			int _loadingValue, _loadingStep, _marginInner, _marginValue, _totalWidth, _totalHeight, _loadingBarX, _loadingBarY;

			bool _isAdvertisement;
			std::string getRandomAdvertisement();
			void loadAdvertisement();
		};
	}
}
