#pragma once
#include <string>
#include "lib\JSONParser.h"

namespace JSON { class JSONDocument; }
namespace sdmg {
	namespace helperclasses {
		class ProgressManager
		{
		public:

			static ProgressManager& getInstance() {
				static ProgressManager _instance;
				return _instance;
			}

			void cleanup();

			void setKey();
			const int getKey();

			void setStatistics(std::string name, std::string key, std::string value);
			JSON::JSONArray &getStatistics();

			void setAutosave(bool enable);
			bool autosaveEnabled();

			void save();
			void load();
			void reset();

		private:
			ProgressManager();
			JSON::JSONDocument *_jsonDoc;

			int getCharacterIndex(std::string name);
		};
	}
}