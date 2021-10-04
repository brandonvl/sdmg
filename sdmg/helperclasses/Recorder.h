#pragma once
#include <string>
#include <queue>
#include <chrono>
#include <map>

namespace sdmg
{
	namespace model {
		class Character;
	}
	namespace engine {
		class GameBase;
	}

	namespace helperclasses {
		class Recorder
		{
		public:
			Recorder();
			~Recorder();

			void start(engine::GameBase &game, const std::string &level);
			void record(std::string action, model::Character &character, bool keyDown);
			void save(std::string path);
			void registerCharacter(model::Character &character);
			void reset() { init(); }
			void gameOver();

		private:
			std::string *_level = nullptr;
			class RecordItem {
			public:
				RecordItem(std::string action, int character, int timestamp, bool keyDown) : _action(action), _character(character), _timestamp(timestamp), _keyDown(keyDown) {}
				~RecordItem() {
					for (auto i : _playerData) delete i;
					_playerData.clear();
				}

				struct PlayerData { 
					int character, hp, lives, pp;
					float x, y, velocityX, velocityY;
					int direction;
				};

				void addPlayerData(PlayerData data) { _playerData.push_back(new PlayerData(data)); }
				std::vector<PlayerData*> &getPlayerData() { return _playerData; }

				const std::string &getAction() { return _action; }
				const int &getCharacter() { return _character; }
				const int &getTimestamp() { return _timestamp; }
				const bool &getKeyDown() { return _keyDown; }
				//const float &getX() { return _x; }
				//const float &getY() { return _y; }
				//const float &getVelocityX() { return _velocityX; }
				//const float &getVelocityY() { return _velocityY; }
				//const int &getHP() { return _hp; }
				//const int &getLives() { return _lives; }
				//const int &getPP() { return _pp; }
			private:
				std::string _action;
				int _character, _timestamp;// , _hp, _lives, _pp;
				bool _keyDown;
				std::vector<PlayerData*> _playerData;
				//float _x, _y, _velocityX, _velocityY;
			};

			std::queue<RecordItem*> *_recordQueue = nullptr;
			std::map<model::Character*, int> *_characters = nullptr;
			std::chrono::high_resolution_clock::time_point *_recordStartTime = nullptr;
			int _characterIndex = 0;
			bool _enabled;

			void init();
			void cleanup();
		};

	}
}