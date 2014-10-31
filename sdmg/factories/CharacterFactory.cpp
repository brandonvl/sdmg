#include "CharacterFactory.h"
#include "model\Character.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\GameBase.h"
#include "engine\Engine.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\World.h"
#include "lib\JSONParser.h"
#include "model\Platform.h"

namespace sdmg {
	namespace factories {
		Character *CharacterFactory::create(const std::string name, engine::GameBase &game, float xPosition, float yPosition) {
			JSON::JSONDocument *doc = JSON::JSONDocument::fromFile("assets/characters/" + name + "/data");
			JSON::JSONObject &obj = doc->getRootObject();
			
			Character *character = new Character();
			character->setSize(obj.getObject("size").getFloat("width"), obj.getObject("size").getFloat("height"));
			character->setSpeed(obj.getObject("speed").getFloat("horizontal"), obj.getObject("speed").getFloat("vertical"));
			character->setDirection(MovableGameObject::Direction::RIGHT);
			character->setSpawnDirection(MovableGameObject::Direction::RIGHT);
			character->setLocation(xPosition, yPosition);
			character->setSpawnLocation(xPosition, yPosition);
			character->setLives(4);
			character->setHP(100);
			character->setName(obj.getString("name"));
			character->setKey(name);
			character->setAttackSize(obj.getObject("attack").getObject("size").getFloat("width"), obj.getObject("attack").getObject("size").getFloat("height"));
			character->setAttackY(obj.getObject("attack").getFloat("position"));
			
			game.getWorld()->addGameObject(character);
			
			loadSpriteMap(character, name, game, obj);

			game.getEngine()->getPhysicsEngine()->addBody(character, 40, 10);

			return character;
		}

		void CharacterFactory::loadSpriteMap(Character *character, const std::string name, engine::GameBase &game, JSON::JSONObject &obj) {
			DrawEngine *drawEngine = game.getEngine()->getDrawEngine();
			float scale = obj.getFloat("scale");
			std::string folder = "assets/characters/" + name + "/";

			drawEngine->loadMap(character, MovableGameObject::State::WALKING, folder + "walking.sprite", obj.getArray("walking").getFloat(0), obj.getArray("walking").getFloat(1), scale);
			drawEngine->loadMap(character, MovableGameObject::State::IDLE, folder + "idle.sprite", obj.getArray("idle").getFloat(0), obj.getArray("idle").getFloat(1), scale);
			drawEngine->loadMap(character, MovableGameObject::State::JUMPING, folder + "jumping.sprite", obj.getArray("jumping").getFloat(0), obj.getArray("jumping").getFloat(1), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::JUMPINGLEFT, folder + "jump_end.sprite", obj.getArray("jumpEnd").getFloat(0), obj.getArray("jumpEnd").getFloat(1), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::JUMPINGRIGHT, folder + "jump_end.sprite", obj.getArray("jumpEnd").getFloat(0), obj.getArray("jumpEnd").getFloat(1), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::FALLING, folder + "falling.sprite", obj.getArray("falling").getFloat(0), obj.getArray("falling").getFloat(1), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::FALLINGLEFT, folder + "falling.sprite", obj.getArray("falling").getFloat(0), obj.getArray("falling").getFloat(1), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::FALLINGRIGHT, folder + "falling.sprite", obj.getArray("falling").getFloat(0), obj.getArray("falling").getFloat(1), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::FORWARD_ROLL, folder + "forward_roll.sprite", obj.getArray("forwardRoll").getFloat(0), obj.getArray("forwardRoll").getFloat(1), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::KNOCKBACKLEFT, folder + "knockback.sprite", obj.getArray("knockback").getFloat(0), obj.getArray("knockback").getFloat(1), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::KNOCKBACKRIGHT, folder + "knockback.sprite", obj.getArray("knockback").getFloat(0), obj.getArray("knockback").getFloat(1), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::KNEELING, folder + "kneeling.sprite", obj.getArray("kneeling").getFloat(0), obj.getArray("kneeling").getFloat(1), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::MIDRANGEATTACKBEGIN, folder + "mid_range_attack_begin.sprite", obj.getArray("midRangeAttackBegin").getFloat(0), obj.getArray("midRangeAttackBegin").getFloat(1), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::MIDRANGEATTACK, folder + "mid_range_attack.sprite", obj.getArray("midRangeAttack").getFloat(0), obj.getArray("midRangeAttack").getFloat(1), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::MIDRANGEATTACKEND, folder + "mid_range_attack_end.sprite", obj.getArray("midRangeAttackEnd").getFloat(0), obj.getArray("midRangeAttackEnd").getFloat(1), scale, Surface::AnimationType::ONCE);
		}
	}
}