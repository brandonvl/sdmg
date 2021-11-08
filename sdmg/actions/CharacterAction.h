#pragma once
#include "engine\input\Action.h"

namespace sdmg {
	namespace model { class Character; }

	using namespace model;
	using namespace engine::input;

	namespace actions {
		class CharacterAction : public Action
		{
		public:
			CharacterAction(Character *character, std::string name);
			virtual ~CharacterAction();
			virtual bool run(engine::GameBase &game) override;
		protected:
			Character *_character = nullptr;
			CharacterAction(Character *character, SDL_Event event, std::string name);
		};
	}
}