//
//
//  @ Project : SDMG
//  @ File Name : LoadingPlayBackState.h
//  @ Date : 18-12-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "engine\GameState.h"
#include "PlayBackState.h"
#include <string>
#include <vector>
#include <queue>
#include <json.hpp>

using namespace sdmg::engine;

namespace sdmg {
	namespace model {
		class Platform;
		class Character;
		class MovablePlatform;
	}
	namespace helperclasses{ class HUD; }
	namespace gamestates {
		class LoadingPlayBackState : public GameState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void pause(GameBase &game);
			void resume(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);
			static LoadingPlayBackState& getInstance() {
				static LoadingPlayBackState _instance;
				return _instance;
			}

			void setPlaybackFileName(std::string fileName);

		protected:
			LoadingPlayBackState() { }
		private:
			GameBase *_game = nullptr;
			void load();
			void loadPlaybackSteps();
			void loadCharacters(std::vector<nlohmann::json>& startingPositions);
			void loadBulletBobs(std::vector<nlohmann::json>& bobs);
			void loadLevel();
			void loadKeybindings();
			void clearEventQueue();
			bool _isLoaded, _isError;
			std::vector<helperclasses::HUD*> *_huds = nullptr;
			std::string *_level, *_fileName = nullptr;
			std::vector<std::string*> *_characters = nullptr;
			std::vector<Character*> *_characterObjects = nullptr;

			std::queue<PlayBackState::RecordStep*> *_recordQueue = nullptr;

			// LoadingBar
			int _loadingValue, _loadingStep, _marginInner, _marginValue, _totalWidth, _totalHeight, _loadingBarX, _loadingBarY;

			bool _isAdvertisement;
			int _advertisementX, _advertisementY;

			void loadAdvertisement();
			std::unordered_map<std::string, Action*> *_recordMap = nullptr;
		};
	}
}