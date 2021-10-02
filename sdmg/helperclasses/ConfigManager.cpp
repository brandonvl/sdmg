#include "ConfigManager.h"
#include "engine/util/FileManager.h"

namespace sdmg
{
	namespace helperclasses
	{

		ConfigManager::ConfigManager()
		{
			load();
		}

		void ConfigManager::cleanup()
		{

		}

		void ConfigManager::load()
		{
			if (_jsonConfig.is_null())
			{
				_jsonConfig = engine::util::FileManager::getInstance().loadFileContentsAsJson("assets/config");
			}
		}

		void ConfigManager::save()
		{
			engine::util::FileManager::getInstance().saveJsonContentToFile("assets/config", _jsonConfig);
		}

		void ConfigManager::setKey(int deviceIndex, std::string action, int key, std::string device)
		{
			_jsonConfig["keybindings"][deviceIndex]["keys"][action] = key;
		}

		const int ConfigManager::getKey(int deviceIndex, std::string action)
		{
			return _jsonConfig["keybindings"][deviceIndex]["keys"][action].get<int>();
		}

		const int ConfigManager::getDeviceIndex(const std::string& deviceName)
		{
			auto &jsonKeyBindings = _jsonConfig["keybindings"];

			for (int i = 0; i < jsonKeyBindings.size(); ++i)
			{
				if (jsonKeyBindings[i]["device"] == deviceName)
					return i;
			}
			return -1;
		}

		const std::string ConfigManager::getUnlockableCharacterName(std::string playerName)
		{
			return _jsonConfig["unlockables"][playerName].get<std::string>();
		}
	}
}