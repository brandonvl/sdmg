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
#include <map>
#include <json.hpp>
struct SDL_Thread;

using namespace sdmg::engine;

namespace sdmg
{
	namespace model
	{
		class Platform;
		class Character;
		class MovablePlatform;
	}
	namespace helperclasses
	{
		class HUD;
	}
	namespace gamestates
	{
		class LoadingSinglePlayerState : public GameState
		{
		public:
			void init(GameBase& game);
			void cleanup(GameBase& game);

			void pause(GameBase& game);
			void resume(GameBase& game);

			void handleEvents(GameBase& game, GameTime& gameTime);
			void update(GameBase& game, GameTime& gameTime);
			void draw(GameBase& game, GameTime& gameTime);
			static LoadingSinglePlayerState& getInstance()
			{
				static LoadingSinglePlayerState _instance;
				return _instance;
			}

			void unloadAll();
			std::string getLevelName();
			std::string getPlayerName();
			void setPlayerName(std::string playerName);
			void setSlotKeyBinding(std::map<std::string, int>* input, std::vector<std::string>* keys);
			void loadNextFight();
			bool hasFinished();

		protected:
			LoadingSinglePlayerState()
			{
			}
		private:
			GameBase* _game;
			void load();
			void loadCharacters();
			void loadBulletBobs(nlohmann::json& bobs);
			void loadLevel(std::string level);
			void loadKeybindings();
			void clearEventQueue();
			void setEnemies();
			bool _isLoaded, _isError;
			std::vector<helperclasses::HUD*>* _huds;
			std::map<std::string, int>* _slotKeyInput;
			std::vector<std::string>* _keys;

			std::string* _playerName, * _levelName;
			std::vector<std::string*>* _enemies;

			int _advertisementX, _advertisementY;
			int _loadingValue, _loadingStep, _marginInner, _marginValue, _totalWidth, _totalHeight, _loadingBarX, _loadingBarY;

			bool _isAdvertisement, _removeFirstEnemy;
			std::string getRandomAdvertisement();
			void loadAdvertisement();
			std::string getSlotKeyInput(int slot);
		};
	}
}
