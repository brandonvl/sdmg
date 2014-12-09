#include "helperclasses\ProgressManager.h"

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <stdio.h>

struct tm newtime;
__time32_t aclock;

using namespace std;

namespace sdmg {
	namespace helperclasses {

		ProgressManager::ProgressManager()
		{
			currentSavegame = -1;
			loaddefaults();
		}

		void ProgressManager::cleanup()
		{
			delete _jsonDoc;
		}

		void ProgressManager::loaddefaults()
		{
			delete _jsonDoc;
			_jsonDoc = JSON::JSONDocument::fromFile("assets/defaultprogress");
		}

		void ProgressManager::reset()
		{
			// Reset timestamp
			_jsonDoc->getRootObject().getArray("savegame").getObject(currentSavegame).getVariable("timestamp").setValue("");
			
			// Reset characters
			for (auto i = 0; i < getStatistics().size(); i++) {
				JSON::JSONObject &characterObj = getStatistics().getObject(i);
				characterObj.getVariable("wins").setValue(std::to_string(0));
				characterObj.getVariable("losses").setValue(std::to_string(0));
				if (characterObj.getString("name") == "Nivek The Assassin" || characterObj.getString("name") == "Fiat Panda")
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
			if (currentSavegame >= 0) {
				setTimestamp(getTimestampNow());
				_jsonDoc->saveFile("assets/progress");
			}
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
			if (currentSavegame < 0)
				return _jsonDoc->getRootObject().getArray("savegame").getObject(0).getArray("characters");
			else
				return _jsonDoc->getRootObject().getArray("savegame").getObject(currentSavegame).getArray("characters");
		}

		int ProgressManager::getCharacterIndex(std::string name)
		{
			int characterIndex = -1;
			JSON::JSONArray &characterArr = _jsonDoc->getRootObject().getArray("savegame").getObject(currentSavegame).getArray("characters");
			for (int i = 0; i < characterArr.size(); i++) {
				JSON::JSONObject &characterObj = characterArr.getObject(i);
				if (name == characterObj.getString("name")) {
					characterIndex = i;
					break;
				}
			}
			return characterIndex;
		}

		bool ProgressManager::autosaveEnabled()
		{
			if (_jsonDoc)
				return _jsonDoc->getRootObject().getBoolean("autosave");
			return false;
		}

		void ProgressManager::setAutosave(bool enable)
		{
			_jsonDoc->getRootObject().getVariable("autosave").setValue(enable ? "true" : "false");
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
			return savegame < _jsonDoc->getRootObject().getArray("savegame").size() && _jsonDoc->getRootObject().getArray("savegame").getObject(savegame).getString("timestamp") != "" ? _jsonDoc->getRootObject().getArray("savegame").getObject(savegame).getString("timestamp") : "";
		}

		std::string ProgressManager::getTimestampNow()
		{
			char buffer[32];
			errno_t errNum;
			_time32(&aclock);   // Get time in seconds.
			_localtime32_s(&newtime, &aclock);   // Convert time to struct tm form.

			// Print local time as a string.

			errNum = asctime_s(buffer, 32, &newtime);
			if (errNum)
			{
				printf("Error code: %d", (int)errNum);
			}
			printf("Current date and time: %s", buffer);
			return buffer;
		}

		JSON::JSONObject ProgressManager::defaultSavegame()
		{
			return JSON::JSONDocument::fromFile("assets/reset")->getRootObject();
		}
	}
}