#pragma once

#include <vector>
#include <lib\JSONParser.h>

namespace sdmg {
	namespace helperclasses {
		class StatisticsManager
		{
		public:

			static StatisticsManager& getInstance() {
				static StatisticsManager _instance;
				return _instance;
			}

			/*void save(std::vector<std::vector<std::string>> statistics);
			std::vector<std::vector<std::string>> load();
			void reset();*/

			void setKey();
			const int getKey();

			void setStatistics(std::string name, std::string key, std::string value);
			const int getStatistics(std::string name, std::string key);

			void save();

		private:
			JSON::JSONDocument *_jsonDoc;
			void load();

			StatisticsManager();
			virtual ~StatisticsManager();

			int getCharacterIndex(std::string name);
		};
	}
}