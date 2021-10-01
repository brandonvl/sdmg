#pragma once
#include <string>
#include <json.hpp>

namespace JSON { class JSONObject; }

namespace sdmg {
	namespace model {
		class Character;
	}
	namespace engine{
		class GameBase;
	}

	namespace factories {
		using namespace model;

		class CharacterFactory
		{
		public:
			static Character *create(const std::string name, engine::GameBase &game, const float &xPosition, const float &yPosition);
		private:
			static void loadSpriteMap(Character *character, const std::string name, engine::GameBase &game, nlohmann::json& json);
		};
	}
}