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
#include <map>
#include <vector>
#include <string>
#include <json.hpp>

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
		class LoadingSurvivalState : public GameState
		{
		public:
			void init(GameBase& game);
			void cleanup(GameBase& game);

			void pause(GameBase& game);
			void resume(GameBase& game);

			void handleEvents(GameBase& game, GameTime& gameTime);
			void update(GameBase& game, GameTime& gameTime);
			void draw(GameBase& game, GameTime& gameTime);
			static LoadingSurvivalState& getInstance()
			{
				static LoadingSurvivalState _instance;
				return _instance;
			}

			std::string getLevelName();
			std::string getPlayerName();
			void setPlayerName(std::string playerName);
			void setSlotKeyBinding(std::map<std::string, int>* input, std::vector<std::string>* keys);

		protected:
			LoadingSurvivalState()
			{
			}
		private:
			GameBase* _game;
			void load();
			void loadCharacters(nlohmann::json& startingPositions);
			void loadBulletBobs(nlohmann::json& bobs);
			void loadLevel();
			void loadKeybindings();
			void clearEventQueue();
			void setEnemies();
			bool _isLoaded, _isError, _isTutorial;
			std::string getSlotKeyInput(int slot);
			std::vector<helperclasses::HUD*>* _huds;

			std::string* _playerName, * _levelName;
			std::vector<std::string*>* _enemyNames;
			std::vector<model::Character*>* _enemies;
			std::map<std::string, int>* _slotKeyInput;
			std::vector<std::string>* _keys;

			// LoadingBar
			int _loadingValue, _loadingStep, _marginInner, _marginValue, _totalWidth, _totalHeight, _loadingBarX, _loadingBarY;

			bool _isAdvertisement;
			int _advertisementX, _advertisementY;
			void loadAdvertisement();
		};
	}
}
