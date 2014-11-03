#pragma once
#include <string>
#include <lib\JSONParser.h>

namespace sdmg {
	namespace helperclasses {
		class ConfigManager
		{
		public:		

			static ConfigManager& getInstance() {
				static ConfigManager *_instance;
				if (_instance == nullptr) _instance = new ConfigManager();
				return *_instance;
			}

			void setKey(int playerIndex, std::string action, int key);
			const int getKey(int playerIndex, std::string action);
			
			void setUserConfig(std::string name, std::string value);
			const std::string getUserConfig(std::string name);

			void save();

		private:
			JSON::JSONDocument *_jsonDoc;
			void load();

			ConfigManager();
			virtual ~ConfigManager();
		};
	}
}