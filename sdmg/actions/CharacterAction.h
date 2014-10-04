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
			CharacterAction(Character *character);
			virtual ~CharacterAction();
		protected:
			Character *_character;
			CharacterAction(Character *character, SDL_Event event);
		};
	}
}