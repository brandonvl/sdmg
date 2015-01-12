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

			void setKey(int playerIndex, std::string action, int key, std::string device);
			const int getKey(int playerIndex, std::string action);
			
			const std::string getDeviceName(int playerIndex);

			void setUserConfig(std::string name, std::string value);
			const std::string getUserConfig(std::string name);
			const int getDeviceIndex(const std::string &deviceName);
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