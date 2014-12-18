#include "CharacterAction.h"
#include "model\Character.h"
#include "helperclasses\Recorder.h"
#include "engine\GameBase.h"

namespace sdmg {
	namespace actions {
		CharacterAction::CharacterAction(Character *character, std::string name) : _character(character), Action(name)
		{
		}

		CharacterAction::CharacterAction(Character *character, SDL_Event event, std::string name) : _character(character), Action(event, name) {}

		CharacterAction::~CharacterAction()
		{
		}

		bool CharacterAction::run(engine::GameBase &game) {
			game.getRecorder().record(_name, *_character, _event.type == SDL_KEYDOWN);
			return true;
		}

	}
}