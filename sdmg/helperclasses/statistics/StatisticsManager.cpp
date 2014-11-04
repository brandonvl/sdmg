#include "StatisticsManager.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

namespace sdmg {
	namespace helperclasses {

		StatisticsManager::StatisticsManager() {
			load();
		}

		StatisticsManager::~StatisticsManager() {
			delete _jsonDoc;
		}

		//const string savegame("assets\\statistics\\statistics");

		/*void StatisticsManager::save(std::vector<std::vector<std::string>> statistics)
		{
			ofstream output_file("assets\\statistics\\statistics");
			for (auto s : statistics)
			{
				output_file << s.at(0) + " " + s.at(1) + " " + s.at(2) + ";\n";
			}
		}

		std::vector<std::vector<std::string>> StatisticsManager::load()
		{
			std::vector<std::vector<std::string>> statistics;
			std::vector<std::string> character;
			
			std::ifstream input_file("assets\\statistics\\statistics");

			std::string line;
			while (getline(input_file, line)) {
				std::string str;
				for (auto c : line) {
					if (c == ' ' || c == ';') {
						character.push_back(str);
						str = "";
					}
					else {
						str += c;
					}
				}
				statistics.push_back(character);
				character.clear();
			}
			return statistics;
		}

		void StatisticsManager::reset()
		{

		}*/

		void StatisticsManager::load() {
			if (_jsonDoc == nullptr)
				_jsonDoc = JSON::JSONDocument::fromFile("assets/statistics/statistics.json");
		}

		void StatisticsManager::save() {
			_jsonDoc->saveFile("assets/statistics/statistics.json");
		}

		void StatisticsManager::setStatistics(std::string name, std::string key, std::string value) {
			JSON::JSONArray &characterArr = _jsonDoc->getRootObject().getArray("characters");
			JSON::JSONObject characterObj = characterArr.getObject(getCharacterIndex(name));
			JSON::JSONVariable *var = new JSON::JSONVariable(&characterObj.getObject(0));
			var->setValue(value);
			var->setValueType(JSON::JSONVariable::ValueType::String);
			characterObj.set(name, *var);
		}

		const int StatisticsManager::getStatistics(std::string name, std::string key) {
			JSON::JSONArray &characterArr = _jsonDoc->getRootObject().getArray("characters");
			return characterArr.getObject(getCharacterIndex(name)).getInt(key);
		}

		int StatisticsManager::getCharacterIndex(std::string name) {
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