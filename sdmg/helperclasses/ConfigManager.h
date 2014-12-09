#pragma once
#include <string>

namespace JSON { class JSONDocument; }
namespace sdmg {
	namespace helperclasses {
		class ConfigManager
		{
		public:		

			static ConfigManager& getInstance() {
				static ConfigManager _instance;
				return _instance;
			}

			void cleanup();

			void setKey(int playerIndex, std::string action, int key);
			const int getKey(int playerIndex, std::string action);
			
			void setUserConfig(std::string name, std::string value);
			const std::string getUserConfig(std::string name);

			const std::string ConfigManager::getUnlockableCharacterName(std::string playerName);

			void save();

		private:
			ConfigManager();
			//~ConfigManager() {};
			JSON::JSONDocument *_jsonDoc;
			void load();

			
		};
	}
}