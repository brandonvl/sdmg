#include "CharacterFactory.h"
#include "model\Character.h"
#include "engine\util\FileParser.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\GameBase.h"
#include "engine\Engine.h"
#include "engine\physics\PhysicsEngine.h"

namespace sdmg {
	namespace factories {
		Character *CharacterFactory::create(const std::string name, engine::GameBase &game) {

			util::FileParser parser;

			if (parser.load("assets/" + name + "/data")) {

				Character *character = new Character();
				character->setSize(parser.getFloat("width"), parser.getFloat("height"));
				character->setSpeed(parser.getFloat("horizontalSpeed"), parser.getFloat("verticalSpeed"));
				character->setDirection(MovableGameObject::Direction::RIGHT);

				loadSpriteMap(character, name, game, parser);

				game.getEngine()->getPhysicsEngine()->addBody(100 + 110 / 2, 10, 110, 143, true, character);

				return character;
			}
			else return nullptr;
		}

		void CharacterFactory::loadSpriteMap(Character *character, const std::string name, engine::GameBase &game, util::FileParser &parser) {
			DrawEngine *drawEngine = game.getEngine()->getDrawEngine();
			float scale = parser.getFloat("scale");
			std::string folder = "assets/" + name + "/";
			drawEngine->loadMap(character, MovableGameObject::State::WALKING, folder + "walking.png", 440.25, 573, scale);
			drawEngine->loadMap(character, MovableGameObject::State::IDLE, folder + "idle.png", 393.125, 548, scale);
			drawEngine->loadMap(character, MovableGameObject::State::JUMPING, folder + "jumping.png", 462.2, 622, scale);
			drawEngine->loadMap(character, MovableGameObject::State::FORWARD_ROLL, folder + "forward_roll.png", 761.14, 608, scale);
		}
	}
}