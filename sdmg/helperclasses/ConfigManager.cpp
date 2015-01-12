#include "ConfigManager.h"
#include <lib/JSONParser.h>

namespace sdmg {
	namespace helperclasses {

		ConfigManager::ConfigManager()
		{
			load();
		}

		void ConfigManager::cleanup()
		{
			delete _jsonDoc;
		}
		
		void ConfigManager::load() {			
			if (_jsonDoc == nullptr) {
				_jsonDoc = JSON::JSONDocument::fromFile("assets/config");
			}
		}

		void ConfigManager::save() {
			_jsonDoc->saveFile("assets/config");
		}

		void ConfigManager::setUserConfig(std::string name, std::string value) {
			JSON::JSONObject &keys = _jsonDoc->getRootObject().getObject("userconfig");
			JSON::JSONVariable *var = new JSON::JSONVariable(&keys);
			var->setValue(value);
			var->setValueType(JSON::JSONVariable::ValueType::String);
			keys.set(name, *var);
			
		}

		const std::string ConfigManager::getUserConfig(std::string name) {
			return _jsonDoc->getRootObject().getObject("userconfig").getString(name);
		}

		void ConfigManager::setKey(int playerIndex, std::string action, int key, std::string device) {
			JSON::JSONObject &binding = _jsonDoc->getRootObject().getArray("keybindings").getObject(playerIndex);
			JSON::JSONObject &keys = binding.getObject("keys");
			JSON::JSONVariable &deviceName = binding.getVariable("device");
			keys.getVariable(action).setValue(std::to_string(key));
			deviceName.setValue(device);
			/*
			JSON::JSONVariable *var = new JSON::JSONVariable(&keys);
			var->setValue(std::to_string(key));
			var->setValueType(JSON::JSONVariable::ValueType::Number);
			keys.set(action, *var);*/
		}

		const int ConfigManager::getKey(int playerIndex, std::string action) {
			return _jsonDoc->getRootObject().getArray("keybindings").getObject(playerIndex).getObject("keys").getInt(action);
		}

		const int ConfigManager::getDeviceIndex(const std::string &deviceName) {

			JSON::JSONArray &jsArray = _jsonDoc->getRootObject().getArray("keybindings");

			for (int i = 0; i < jsArray.size(); ++i) {
				if (jsArray.getObject(i).getString("device") == deviceName)
					return i;
			}

			return -1;

		}

		const std::string ConfigManager::getDeviceName(int playerIndex){
			return _jsonDoc->getRootObject().getArray("keybindings").getObject(playerIndex).getVariable("device").getString();
		}

		const std::string ConfigManager::getUnlockableCharacterName(std::string playerName) {
			return _jsonDoc->getRootObject().getObject("unlockables").getString(playerName);
		}
	}
}