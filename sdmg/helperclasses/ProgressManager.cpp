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

namespace sdmg {
	namespace helperclasses {

		ProgressManager::ProgressManager()
		{
			currentSavegame = -1;
			load();
		}

		void ProgressManager::cleanup()
		{
			delete _jsonDoc;
		}

		void ProgressManager::reset()
		{
			// Reset autosave
			setAutosave(true);

			// Reset timestamp
			setTimestamp("");

			// Reset characters
			for (auto i = 0, ilen = getStatistics().size(); i < ilen; ++i) {
				JSON::JSONObject &characterObj = getStatistics().getObject(i);
				characterObj.getVariable("wins").setValue(std::to_string(0));
				characterObj.getVariable("losses").setValue(std::to_string(0));
				if (characterObj.getBoolean("persistant") == true)
					characterObj.getVariable("unlocked").setValue(true);
				else
					characterObj.getVariable("unlocked").setValue(false);
			}

			// Reset levels
			for (auto i = 0, ilen = getLevels().size(); i < ilen; ++i) {
				auto &levelObj = getLevels().getObject(i);
				if (levelObj.getBoolean("persistant") == true)
					levelObj.getVariable("unlocked").setValue(true);
				else
					levelObj.getVariable("unlocked").setValue(false);
			}
		}

		void ProgressManager::load()
		{
			try{
				delete _jsonDoc;
				_jsonDoc = JSON::JSONDocument::fromFile("assets/progress");
			}
			catch (...) {
				try{
					delete _jsonDoc;
					_jsonDoc = JSON::JSONDocument::fromFile("assets/defaultprogress");
					_jsonDoc->saveFile("assets/progress");
				}
				catch (...) {
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Cannot load progress", "Cannot load progress", NULL);
				}
			}
		}

		void ProgressManager::save()
		{
			_jsonDoc->saveFile("assets/progress");
		}

		void ProgressManager::setStatistics(std::string name, std::string key, std::string value)
		{
			JSON::JSONArray &characterArr = _jsonDoc->getRootObject().getArray("savegame").getObject(currentSavegame).getArray("characters");
			JSON::JSONObject characterObj = characterArr.getObject(getCharacterIndex(name));
			characterObj.getVariable(key).setValue(value);
		}

		JSON::JSONArray &ProgressManager::getStatistics()
		{
			return _jsonDoc->getRootObject().getArray("savegame").getObject(currentSavegame).getArray("characters");
		}

		void ProgressManager::setLevel(std::string name, std::string key, std::string value)
		{
			JSON::JSONArray &levelArr = _jsonDoc->getRootObject().getArray("savegame").getObject(currentSavegame).getArray("levels");
			JSON::JSONObject levelObj = levelArr.getObject(getLevelIndex(name));
			levelObj.getVariable(key).setValue(value);
		}

		JSON::JSONArray &ProgressManager::getLevels()
		{
			return _jsonDoc->getRootObject().getArray("savegame").getObject(currentSavegame).getArray("levels");
		}

		std::vector<std::vector<std::string>> *ProgressManager::getHighscores()
		{
			std::vector<std::vector<std::string>> *highscores = new std::vector<std::vector<std::string>>();
			JSON::JSONArray &jHighscores = _jsonDoc->getRootObject().getArray("highscores");
			for (auto i = 0, ilen = jHighscores.size(); i < ilen; ++i) {
				highscores->push_back({ jHighscores.getObject(i).getString("number"), jHighscores.getObject(i).getString("name"), jHighscores.getObject(i).getString("score") });
			}
			return highscores;
		}

		void ProgressManager::setHighscore(int index, std::string key, std::string value)
		{
			JSON::JSONArray &highscoreArr = _jsonDoc->getRootObject().getArray("highscores");
			JSON::JSONObject &highscoreObj = highscoreArr.getObject(index);
			highscoreObj.getVariable(key).setValue(value);
		}

		void ProgressManager::addHighscore(std::string initials, int highscore)
		{
			auto &highscoreArr = _jsonDoc->getRootObject().getArray("highscores");

			int index = -1;

			for (int i = highscoreArr.size() - 2; i >= 0; --i) {
				// Down rank score
				setHighscore(i + 1, "name", highscoreArr.getObject(i).getString("name"));
				setHighscore(i + 1, "score", highscoreArr.getObject(i).getString("score"));

				if (highscore < stoi(highscoreArr.getObject(i + 1).getString("score"))) { break; }

				index = i;				
			} 
				
			// Set new highscore at current position
			setHighscore(index, "name", initials);
			setHighscore(index, "score", std::to_string(highscore));
		}

		int ProgressManager::getLowestHighscore()
		{
			std::vector<std::vector<std::string>> *highscores = getHighscores();
			int lowest = stoi(highscores->back().back());
			delete highscores;
			return lowest;
		}

		int ProgressManager::getCharacterIndex(std::string name)
		{
			int characterIndex = -1;
			JSON::JSONArray &characterArr = _jsonDoc->getRootObject().getArray("savegame").getObject(currentSavegame).getArray("characters");
			for (int i = 0; i < characterArr.size(); i++) {
				JSON::JSONObject &characterObj = characterArr.getObject(i);
				if (name == characterObj.getString("name") || name == characterObj.getString("key")) {
					characterIndex = i;
					break;
				}
			}
			return characterIndex;
		}

		int ProgressManager::getLevelIndex(std::string name)
		{
			int characterIndex = -1;
			JSON::JSONArray &levelArr = _jsonDoc->getRootObject().getArray("savegame").getObject(currentSavegame).getArray("levels");
			for (int i = 0; i < levelArr.size(); i++) {
				JSON::JSONObject &levelObj = levelArr.getObject(i);
				if (name == levelObj.getString("name") || name == levelObj.getString("key")) {
					characterIndex = i;
					break;
				}
			}
			return characterIndex;
		}

		bool ProgressManager::autosaveEnabled()
		{
			return _jsonDoc->getRootObject().getBoolean("autosave");
		}

		void ProgressManager::setAutosave(bool enable)
		{
			_jsonDoc->getRootObject().getVariable("autosave").setValue(enable ? true : false);
		}

		std::string ProgressManager::getTimestamp()
		{
			return _jsonDoc->getRootObject().getArray("savegame").getObject(currentSavegame).getString("timestamp");
		}

		void ProgressManager::setTimestamp(std::string time)
		{
			_jsonDoc->getRootObject().getArray("savegame").getObject(currentSavegame).getVariable("timestamp").setValue(time);
		}

		std::string ProgressManager::getSaveGameTimestamp(int savegame)
		{
			int numberOfSlots = _jsonDoc->getRootObject().getArray("savegame").size();
			std::string timestamp = _jsonDoc->getRootObject().getArray("savegame").getObject(savegame).getString("timestamp");
			return savegame < numberOfSlots && timestamp != "" ? timestamp : "";
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
			return _jsonDoc->getRootObject().getArray("savegame").getObject(currentSavegame).getArray("characters").getObject(getCharacterIndex(name)).getBoolean("unlocked");
		}

		bool ProgressManager::isUnlockedLevel(std::string name)
		{
			return _jsonDoc->getRootObject().getArray("savegame").getObject(currentSavegame).getArray("levels").getObject(getLevelIndex(name)).getBoolean("unlocked");
		}

		bool ProgressManager::isUnlockableLevel(std::string name)
		{
			return getLevelIndex(name) >= 0;
		}

		void ProgressManager::setIsUnlockedCharacter(std::string name, bool unlocked)
		{
			_jsonDoc->getRootObject().getArray("savegame").getObject(currentSavegame).getArray("characters").getObject(getCharacterIndex(name)).getVariable("unlocked").setValue(unlocked);
		}

		void ProgressManager::setIsUnlockedLevel(std::string name, bool unlocked)
		{
			_jsonDoc->getRootObject().getArray("savegame").getObject(currentSavegame).getArray("levels").getObject(getLevelIndex(name)).getVariable("unlocked").setValue(unlocked);
		}
	}
}