#include "helperclasses\ProgressManager.h"

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <stdio.h>
#include <sstream>

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
			setAutosave(false);

			// Reset timestamp
			setTimestamp("");

			// Reset characters
			for (auto i = 0, ilen = getStatistics().size(); i < ilen; ++i) {
				JSON::JSONObject &characterObj = getStatistics().getObject(i);
				characterObj.getVariable("wins").setValue(std::to_string(0));
				characterObj.getVariable("losses").setValue(std::to_string(0));
				if (characterObj.getString("key") == "nivek" || characterObj.getString("key") == "fiat")
					characterObj.getVariable("unlocked").setValue(true);
				else
					characterObj.getVariable("unlocked").setValue(false);
			}

			// Reset levels
			//TODO
		}

		void ProgressManager::load()
		{
			delete _jsonDoc;
			_jsonDoc = JSON::JSONDocument::fromFile("assets/progress");
		}

		void ProgressManager::save()
		{
			_jsonDoc->saveFile("assets/progress");

		}

		void ProgressManager::setStatistics(std::string name, std::string key, std::string value)
		{
			JSON::JSONArray &characterArr = _jsonDoc->getRootObject().getArray("savegame").getObject(currentSavegame).getArray("characters");
			JSON::JSONObject characterObj = characterArr.getObject(getCharacterIndex(name));
			JSON::JSONVariable *var = new JSON::JSONVariable(&characterObj.getObject(0));
			var->setValue(value);
			var->setValueType(JSON::JSONVariable::ValueType::String);
			characterObj.set(name, *var);
		}

		JSON::JSONArray &ProgressManager::getStatistics()
		{
			return _jsonDoc->getRootObject().getArray("savegame").getObject(currentSavegame).getArray("characters");
		}

		void ProgressManager::addHighscore(std::string initials, int highscore)
		{
			std::string val;
			auto &highscores = _jsonDoc->getRootObject().getArray("highscores");
			for (auto i = 0, ilen = highscores.size(); i < ilen; ++i) {
				if (stoi(highscores.getObject(i).getString("score")) < highscore) {
					for (auto j = i, jlen = highscores.size(); j < jlen; ++j) {
						JSON::JSONVariable *var = new JSON::JSONVariable(&highscores.getObject(j));
						var->setValueType(JSON::JSONVariable::ValueType::String);
						// Set number
						val = std::to_string(stoi(highscores.getObject(j).getString("number")) + 1);
						var->setValue(val);
						if (j + 1 < jlen)
							highscores.getObject(j + 1).set("number", *var);

						// Set initials
						val = highscores.getObject(j).getString("name");
						var->setValue(val);
						if (j + 1 < jlen)
							highscores.getObject(j + 1).set("name", *var);

						// Set score
						val = highscores.getObject(j).getString("score");
						var->setValue(val);
						if (j + 1 < jlen)
							highscores.getObject(j + 1).set("score", *var);
					}
					JSON::JSONVariable *var = new JSON::JSONVariable(&highscores.getObject(i));
					var->setValueType(JSON::JSONVariable::ValueType::String);
					// Set number
					val = highscores.getObject(i).getString("number");
					var->setValue(val);
					highscores.getObject(i).set("number", *var);

					// Set initials
					var->setValue(initials);
					highscores.getObject(i).set("name", *var);

					// Set score
					var->setValue(std::to_string(highscore));
					highscores.getObject(i).set("score", *var);
					break;
				}
			}
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

		int ProgressManager::getLowestHighscore()
		{
			return stoi(getHighscores()->back().back());
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
			JSON::JSONArray &characterArr = _jsonDoc->getRootObject().getArray("savegame").getObject(currentSavegame).getArray("levels");
			for (int i = 0; i < characterArr.size(); i++) {
				JSON::JSONObject &characterObj = characterArr.getObject(i);
				if (name == characterObj.getString("name") || name == characterObj.getString("key")) {
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
			char buffer[32];
			errno_t errNum;
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

			/*errNum = asctime_s(buffer, 32, &newtime);
			if (errNum)
			{
			printf("Error code: %d", (int)errNum);
			}
			printf("Current date and time: %s", buffer);*/
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