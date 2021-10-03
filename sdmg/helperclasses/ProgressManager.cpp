#include "helperclasses\ProgressManager.h"

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <stdio.h>
#include <sstream>
#include <SDL.h>
#include "engine/util/FileManager.h"

struct tm newtime;
__time32_t aclock;

using namespace std;

namespace sdmg
{
	namespace helperclasses
	{

		ProgressManager::ProgressManager()
		{
			currentSavegame = -1;
			load();
		}

		void ProgressManager::cleanup()
		{

		}

		void ProgressManager::reset()
		{
			// Reset autosave
			setAutosave(true);

			// Reset timestamp
			setTimestamp("");

			// Reset characters
			for (auto& characterObject : getStatistics())
			{
				characterObject["wins"] = 0;
				characterObject["losses"] = 0;

				if (characterObject["persistant"].get<bool>() == true)
					characterObject["unlocked"] = true;
				else
					characterObject["unlocked"] = false;
			}

			// Reset levels
			for (auto& levelObject : getLevels())
			{
				if (levelObject["persistant"].get<bool>() == true)
					levelObject["unlocked"] = true;
				else
					levelObject["unlocked"] = false;
			}
		}

		void ProgressManager::load()
		{
			try
			{
				auto progressExists = engine::util::FileManager::getInstance().fileExists("assets/progress");

				if (progressExists)
				{
					_jsonDoc = engine::util::FileManager::getInstance().loadFileContentsAsJson("assets/progress");
				}
				else
				{
					_jsonDoc = engine::util::FileManager::getInstance().loadFileContentsAsJson("assets/defaultprogress");
					engine::util::FileManager::getInstance().saveJsonContentToFile("assets/progress", _jsonDoc);
				}
			}
			catch (...)
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Cannot load progress", "Cannot load progress", NULL);
			}
		}

		void ProgressManager::save()
		{
			engine::util::FileManager::getInstance().saveJsonContentToFile("assets/progress", _jsonDoc);
		}

		void ProgressManager::setStatistics(std::string name, std::string key, std::string value)
		{
			_jsonDoc["savegame"][currentSavegame]["characters"][getLevelIndex(name)][key] = value;
		}

		nlohmann::json& ProgressManager::getStatistics()
		{
			return _jsonDoc["savegame"][currentSavegame]["characters"];
		}

		void ProgressManager::setLevel(std::string name, std::string key, std::string value)
		{
			_jsonDoc["savegame"][currentSavegame]["levels"][getLevelIndex(name)][key] = value;
		}

		nlohmann::json& ProgressManager::getLevels()
		{
			return _jsonDoc["savegame"][currentSavegame]["levels"];
		}

		std::vector<std::vector<std::string>>* ProgressManager::getHighscores()
		{
			std::vector<std::vector<std::string>>* highscores = new std::vector<std::vector<std::string>>();
			auto& jsonHighScores = _jsonDoc["highscores"];

			for (auto& jsonHighscore : jsonHighScores)
			{
				highscores->push_back({ jsonHighscore["number"].get<std::string>(), jsonHighscore["name"].get<std::string>(), jsonHighscore["score"].get<std::string>() });
			}

			return highscores;
		}

		void ProgressManager::setHighscore(int index, std::string key, std::string value)
		{
			_jsonDoc["highscores"][index][key] = value;
		}

		void ProgressManager::addHighscore(std::string initials, int highscore)
		{
			auto& jsonHighScores = _jsonDoc["highscores"];

			int index = -1;

			for (int i = jsonHighScores.size() - 2; i >= 0; --i)
			{
				// Down rank score
				setHighscore(i + 1, "name", jsonHighScores[i]["name"]);
				setHighscore(i + 1, "score", jsonHighScores[i]["score"]);

				auto higherIndex = i + 1;
				if (highscore < stoi(jsonHighScores[higherIndex]["score"].get<std::string>()))
				{
					break;
				}

				index = i;
			}

			// Set new highscore at current position
			setHighscore(index, "name", initials);
			setHighscore(index, "score", std::to_string(highscore));
		}

		int ProgressManager::getLowestHighscore()
		{
			std::vector<std::vector<std::string>>* highscores = getHighscores();
			int lowest = stoi(highscores->back().back());
			delete highscores;
			return lowest;
		}

		int ProgressManager::getCharacterIndex(std::string name)
		{
			auto& jsonCharacters = _jsonDoc["savegame"][currentSavegame]["characters"];

			for (int i = 0; i < jsonCharacters.size(); i++)
			{
				if (jsonCharacters[i]["name"].get<std::string>() == name || jsonCharacters[i]["key"].get<std::string>() == name)
					return i;
			}

			return -1;
		}

		int ProgressManager::getLevelIndex(std::string name)
		{
			auto& jsonLevels = _jsonDoc["savegame"][currentSavegame]["levels"];

			for (int i = 0; i < jsonLevels.size(); i++)
			{
				if (jsonLevels[i]["name"].get<std::string>() == name || jsonLevels[i]["key"].get<std::string>() == name)
					return i;
			}

			return -1;
		}

		bool ProgressManager::autosaveEnabled()
		{
			return _jsonDoc["autosave"].get<bool>();
		}

		void ProgressManager::setAutosave(bool enable)
		{
			_jsonDoc["autosave"] = enable;
		}

		std::string ProgressManager::getTimestamp()
		{
			return _jsonDoc["savegame"][currentSavegame]["timestamp"].get<std::string>();
		}

		void ProgressManager::setTimestamp(std::string time)
		{
			_jsonDoc["savegame"][currentSavegame]["timestamp"] = time;
		}

		std::string ProgressManager::getSaveGameTimestamp(int savegame)
		{
			int numberOfSlots = _jsonDoc["savegame"].size();
			std::string timestamp = _jsonDoc["savegame"][savegame]["timestamp"].get<std::string>();
			return savegame < numberOfSlots&& timestamp != "" ? timestamp : "";
		}

		std::string ProgressManager::getTimestampNow()
		{
			_time32(&aclock);   // Get time in seconds.
			_localtime32_s(&newtime, &aclock);   // Convert time to struct tm form.

			// Print local time as a string.
			char timestamp[32];
			int year = newtime.tm_year + 1900;
			int month = newtime.tm_mon + 1;
			int day = newtime.tm_mday;

			int hour = newtime.tm_hour;
			int minutes = newtime.tm_min;
			int seconds = newtime.tm_sec;
			sprintf_s(timestamp, "%d/%d/%d %d:%d:%d", year, month, day, hour, minutes, seconds);

			return timestamp;
		}

		bool ProgressManager::isUnlockedCharacter(std::string name)
		{
			return _jsonDoc["savegame"][currentSavegame]["characters"][getCharacterIndex(name)]["unlocked"].get<bool>();
		}

		bool ProgressManager::isUnlockedLevel(std::string name)
		{
			return _jsonDoc["savegame"][currentSavegame]["levels"][getLevelIndex(name)]["unlocked"].get<bool>();
		}

		bool ProgressManager::isUnlockableLevel(std::string name)
		{
			return getLevelIndex(name) >= 0;
		}

		void ProgressManager::setIsUnlockedCharacter(std::string name, bool unlocked)
		{
			_jsonDoc["savegame"][currentSavegame]["characters"][getCharacterIndex(name)]["unlocked"] = unlocked;
		}

		void ProgressManager::setIsUnlockedLevel(std::string name, bool unlocked)
		{
			_jsonDoc["savegame"][currentSavegame]["levels"][getLevelIndex(name)]["unlocked"] = unlocked;
		}
	}
}