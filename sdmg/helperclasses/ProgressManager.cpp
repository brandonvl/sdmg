#include "helperclasses\ProgressManager.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

namespace sdmg {
	namespace helperclasses {

		ProgressManager::ProgressManager() {
			reset();
		}

		void ProgressManager::cleanup() {
			delete _jsonDoc;
		}

		void ProgressManager::reset() {
			_jsonDoc = JSON::JSONDocument::fromFile("assets/reset");
		}

		void ProgressManager::load() {
			_jsonDoc = JSON::JSONDocument::fromFile("assets/progress");
		}

		void ProgressManager::save() {
			_jsonDoc->saveFile("assets/progress");
		}

		void ProgressManager::setStatistics(std::string name, std::string key, std::string value) {
			JSON::JSONArray &characterArr = _jsonDoc->getRootObject().getArray("characters");
			JSON::JSONObject characterObj = characterArr.getObject(getCharacterIndex(name));
			JSON::JSONVariable *var = new JSON::JSONVariable(&characterObj.getObject(0));
			var->setValue(value);
			var->setValueType(JSON::JSONVariable::ValueType::String);
			characterObj.set(name, *var);
		}

		JSON::JSONArray &ProgressManager::getStatistics() {
			return _jsonDoc->getRootObject().getArray("characters");
		}

		int ProgressManager::getCharacterIndex(std::string name) {
			int characterIndex = -1;
			JSON::JSONArray &characterArr = _jsonDoc->getRootObject().getArray("characters");
			for (int i = 0; i < characterArr.size(); i++) {
				JSON::JSONObject &characterObj = characterArr.getObject(i);
				if (name == characterObj.getString("name")) {
					characterIndex = i;
					break;
				}
			}
			return characterIndex;
		}

		void setKey() {

		}

		const int getKey() {
			return 0;
		}
	}
}