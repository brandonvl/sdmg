#include "CharacterFactory.h"
#include "model\Character.h"
#include "engine\util\FileParser.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\GameBase.h"
#include "engine\Engine.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\World.h"
#include "model\Platform.h"

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
				character->setLives(4);
				character->setHP(100);
				character->setName(parser.getString("name"));
				character->setKey(name);
				character->setAttackSize(parser.getArray("attackSize")[0], parser.getArray("attackSize")[1]);
				character->setAttackY(parser.getArray("attackY")[0]);

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

			drawEngine->loadMap(character, MovableGameObject::State::KNOCKBACKLEFT, folder + "knockback.sprite", parser.getArray("knockback")[0], parser.getArray("knockback")[1], scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::KNOCKBACKRIGHT, folder + "knockback.sprite", parser.getArray("knockback")[0], parser.getArray("knockback")[1], scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::KNEELING, folder + "kneeling.sprite", parser.getArray("kneeling")[0], parser.getArray("kneeling")[1], scale, Surface::AnimationType::ONCE);

			drawEngine->loadMap(character, MovableGameObject::State::MIDRANGEATTACKBEGIN, folder + "mid_range_attack_begin.sprite", parser.getArray("midRangeAttackBegin")[0], parser.getArray("midRangeAttackBegin")[1], scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::MIDRANGEATTACK, folder + "mid_range_attack.sprite", parser.getArray("midRangeAttack")[0], parser.getArray("midRangeAttack")[1], scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::MIDRANGEATTACKEND, folder + "mid_range_attack_end.sprite", parser.getArray("midRangeAttackEnd")[0], parser.getArray("midRangeAttackEnd")[1], scale, Surface::AnimationType::ONCE);
		
		}
	}
}