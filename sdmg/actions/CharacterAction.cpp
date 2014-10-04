#include "CharacterAction.h"
#include "model\Character.h"

namespace sdmg {
	namespace actions {
		CharacterAction::CharacterAction(Character *character) : _character(character)
		{
		}

		CharacterAction::CharacterAction(Character *character, SDL_Event event) : _character(character), Action(event) {}

		CharacterAction::~CharacterAction()
		{
		}
	}
}