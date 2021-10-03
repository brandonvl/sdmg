#pragma once
#include <string>
#include <json.hpp>

namespace sdmg
{
	namespace helperclasses
	{
		class ProgressManager
		{
		public:

			static ProgressManager& getInstance()
			{
				static ProgressManager _instance;
				return _instance;
			}

			void cleanup();

			void setStatistics(std::string name, std::string key, std::string value);
			nlohmann::json& getStatistics();

			void setLevel(std::string name, std::string key, std::string value);
			nlohmann::json& getLevels();

			std::vector<std::vector<std::string>>* getHighscores();
			void setHighscore(int index, std::string key, std::string value);
			void addHighscore(std::string initials, int highscore);
			int getLowestHighscore();

			void setAutosave(bool enable);
			bool autosaveEnabled();

			std::string getTimestamp();
			void setTimestamp(std::string time);

			void save();
			void load();
			void reset();

			std::string getSaveGameTimestamp(int savegame);
			std::string getTimestampNow();

			int currentSavegame;

			bool isUnlockedCharacter(std::string name);
			bool isUnlockedLevel(std::string name);
			bool isUnlockableLevel(std::string name);
			void setIsUnlockedCharacter(std::string name, bool unlocked);
			void setIsUnlockedLevel(std::string name, bool unlocked);

		private:
			ProgressManager();
			nlohmann::json _jsonDoc;

			int getCharacterIndex(std::string name);
			int getLevelIndex(std::string name);
		};
	}
}