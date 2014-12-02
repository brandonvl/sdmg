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

			void setStatistics(std::string name, std::string key, std::string value);
			JSON::JSONArray &getStatistics();

			void setAutosave(bool enable);
			bool autosaveEnabled();

			std::string getTimestamp();
			void setTimestamp(std::string time);

			void save();
			void load();
			void reset();
			void loaddefaults();

			std::string getSaveGameTimestamp(int savegame);
			std::string getTimestampNow();

			int currentSavegame;

			JSON::JSONObject defaultSavegame();

		private:
			ProgressManager();
			JSON::JSONDocument *_jsonDoc;

			int getCharacterIndex(std::string name);
		};
	}
}