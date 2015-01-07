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

		private:
			std::string *_level;
			class RecordItem {
			public:
				RecordItem(std::string action, int character, int timestamp, bool keyDown) : _action(action), _character(character), _timestamp(timestamp), _keyDown(keyDown) {}

				const std::string &getAction() { return _action; }
				const int &getCharacter() { return _character; }
				const int &getTimestamp() { return _timestamp; }
				const bool &getKeyDown() { return _keyDown; }
			private:
				std::string _action;
				int _character, _timestamp;
				bool _keyDown;
			};

			std::queue<RecordItem*> *_recordQueue;
			std::map<model::Character*, int> *_characters;
			std::chrono::high_resolution_clock::time_point *_recordStartTime;
			int _characterIndex = 0;
			bool _enabled;

			void init();
			void cleanup();
		};

	}
}