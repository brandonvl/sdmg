#pragma once
#include <string>
#include <json.hpp>

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

			void setKey(int deviceIndex, std::string action, int key, std::string device);
			const int getKey(int deviceIndex, std::string action);
			
			const int getDeviceIndex(const std::string &deviceName);
			const std::string ConfigManager::getUnlockableCharacterName(std::string playerName);

			void save();

		private:
			ConfigManager();
			~ConfigManager() = default;
			nlohmann::json _jsonConfig;
			void load();

			
		};
	}
}