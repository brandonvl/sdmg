/*
#pragma once
class LoadingSinglePlayerState
{
public:
	LoadingSinglePlayerState();
	virtual ~LoadingSinglePlayerState();
};

//
//
//  @ Project : SDMG
//  @ File Name : LoadingState.h
//  @ Date : 26-9-2014
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

			void setLevel(std::string *level);

		protected:
			LoadingSinglePlayerState() { }
		private:
			GameBase *_game;
			void load();
			void loadCharacters(JSON::JSONArray &startingPositions);
			void loadBulletBobs(JSON::JSONArray &bobs);
			void loadLevel();
			void loadKeybindings();
			bool _isLoaded, _isError;
			std::vector<helperclasses::HUD*> *_huds;
			std::vector<std::string*> *enemies;
			std::string *_level, *_progress, *player;
			int _advertisementX, _advertisementY;

			// LoadingBar
			int _loadingValue, _loadingStep, _marginInner, _marginValue, _totalWidth, _totalHeight, _loadingBarX, _loadingBarY;

			bool _isAdvertisement;
			std::string getRandomAdvertisement();
			void loadAdvertisement();
		};
	}
}
*/