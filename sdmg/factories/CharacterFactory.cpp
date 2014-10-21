#include "CharacterFactory.h"
#include "model\Character.h"
#include "engine\util\FileParser.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\GameBase.h"
#include "engine\Engine.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\World.h"

namespace sdmg {
	namespace factories {
		Character *CharacterFactory::create(const std::string name, engine::GameBase &game, float xPosition, float yPosition) {

			util::FileParser parser;

			if (parser.load("assets/characters/" + name + "/data")) {

				Character *character = new Character();
				character->setSize(parser.getFloat("width"), parser.getFloat("height"));
				character->setSpeed(parser.getFloat("horizontalSpeed"), parser.getFloat("verticalSpeed"));
				character->setDirection(MovableGameObject::Direction::RIGHT);
				character->setSpawnDirection(MovableGameObject::Direction::RIGHT);
				character->setLocation(xPosition, yPosition);
				character->setSpawnLocation(xPosition, yPosition);
				character->setLives(5);
				character->setHP(100);
				character->setName(parser.getString("name"));
				character->setKey(name);
				game.getWorld()->addGameObject(character);

				loadSpriteMap(character, name, game, parser);

				game.getEngine()->getPhysicsEngine()->addBody(character, 40, 10);

				return character;
			}
			else return nullptr;
		}

		void CharacterFactory::loadSpriteMap(Character *character, const std::string name, engine::GameBase &game, util::FileParser &parser) {
			DrawEngine *drawEngine = game.getEngine()->getDrawEngine();
			float scale = parser.getFloat("scale");
			std::string folder = "assets/characters/" + name + "/";
			
			drawEngine->loadMap(character, MovableGameObject::State::WALKING, folder + "walking.sprite", parser.getArray("walking")[0], parser.getArray("walking")[1], scale);
			drawEngine->loadMap(character, MovableGameObject::State::IDLE, folder + "idle.sprite", parser.getArray("idle")[0], parser.getArray("idle")[1], scale);
			drawEngine->loadMap(character, MovableGameObject::State::JUMPING, folder + "jumping.sprite", parser.getArray("jumping")[0], parser.getArray("jumping")[1], scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::JUMPINGLEFT, folder + "jump_end.sprite", parser.getArray("jumpEnd")[0], parser.getArray("jumpEnd")[1], scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::JUMPINGRIGHT, folder + "jump_end.sprite", parser.getArray("jumpEnd")[0], parser.getArray("jumpEnd")[1], scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::FALLING, folder + "falling.sprite", parser.getArray("falling")[0], parser.getArray("falling")[1], scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::FALLINGLEFT, folder + "falling.sprite", parser.getArray("falling")[0], parser.getArray("falling")[1], scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::FALLINGRIGHT, folder + "falling.sprite", parser.getArray("falling")[0], parser.getArray("falling")[1], scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::FORWARD_ROLL, folder + "forward_roll.sprite", parser.getArray("forwardRoll")[0], parser.getArray("forwardRoll")[1], scale, Surface::AnimationType::ONCE);
		}
	}
}