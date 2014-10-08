#pragma once
#include <string>

namespace sdmg {
	namespace model {
		class Character;
	}
	namespace engine{
		class GameBase;

		namespace util{
			class FileParser;
		}
	}

	namespace factories {
		using namespace model;

		class CharacterFactory
		{
		public:
			static Character *create(const std::string name, engine::GameBase &game, float xPosition, float yPosition);
		private:
			static void loadSpriteMap(Character *character, const std::string name, engine::GameBase &game, engine::util::FileParser &parser);
		};
	}
}