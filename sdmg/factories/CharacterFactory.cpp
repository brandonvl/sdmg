#include "CharacterFactory.h"
#include "model\Character.h"
#include "engine\util\FileParser.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\GameBase.h"
#include "engine\Engine.h"
#include "engine\physics\PhysicsEngine.h"

namespace sdmg {
	namespace factories {
		Character *CharacterFactory::create(const std::string name, engine::GameBase &game, float xPosition, float yPosition) {

			util::FileParser parser;

			if (parser.load("assets/" + name + "/data")) {

				Character *character = new Character();
				character->setSize(parser.getFloat("width"), parser.getFloat("height"));
				character->setSpeed(parser.getFloat("horizontalSpeed"), parser.getFloat("verticalSpeed"));
				character->setDirection(MovableGameObject::Direction::RIGHT);
				character->setLocation(xPosition, yPosition);

				loadSpriteMap(character, name, game, parser);

				game.getEngine()->getPhysicsEngine()->addBody(character, 40, 10);

				return character;
			}
			else return nullptr;
		}

		void CharacterFactory::loadSpriteMap(Character *character, const std::string name, engine::GameBase &game, util::FileParser &parser) {
			DrawEngine *drawEngine = game.getEngine()->getDrawEngine();
			float scale = parser.getFloat("scale");
			std::string folder = "assets/" + name + "/";
			
			drawEngine->loadMap(character, MovableGameObject::State::WALKING, folder + "walking.png", parser.getArray("walking")[0], parser.getArray("walking")[1], scale);
			drawEngine->loadMap(character, MovableGameObject::State::IDLE, folder + "idle.png", parser.getArray("idle")[0], parser.getArray("idle")[1], scale);
			drawEngine->loadMap(character, MovableGameObject::State::JUMPING, folder + "jumping.png", parser.getArray("jumping")[0], parser.getArray("jumping")[1], scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::FORWARD_ROLL, folder + "forward_roll.png", parser.getArray("forwardRoll")[0], parser.getArray("forwardRoll")[1], scale, Surface::AnimationType::ONCE);
		}
	}
}