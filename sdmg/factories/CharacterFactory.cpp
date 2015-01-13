#include "CharacterFactory.h"
#include "model\Character.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\GameBase.h"
#include "engine\Engine.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\World.h"
#include "lib\JSONParser.h"
#include "model\Platform.h"
#include "engine\audio\AudioEngine.h"

namespace sdmg {
	namespace factories {
		Character *CharacterFactory::create(const std::string name, engine::GameBase &game,const float &xPosition, const float &yPosition) {
			JSON::JSONDocument *doc = JSON::JSONDocument::fromFile("assets/characters/" + name + "/data");
			JSON::JSONObject &obj = JSON::JSONObject(nullptr);
			obj = doc->getRootObject();
			Character *character = new Character();
			character->setSize(obj.getObject("size").getFloat("width"), obj.getObject("size").getFloat("height"));
			character->setSpeed(obj.getObject("speed").getFloat("horizontal"), obj.getObject("speed").getFloat("vertical"));
			character->setLocation(xPosition, yPosition);
			character->setSpawnLocation(xPosition, yPosition);

			if (xPosition > game.getEngine()->getDrawEngine()->getWindowWidth() / 2)
			{
				character->setSpawnDirection(MovableGameObject::Direction::LEFT);
				character->setDirection(MovableGameObject::Direction::LEFT);
			}
			else
			{
				character->setSpawnDirection(MovableGameObject::Direction::RIGHT);
				character->setDirection(MovableGameObject::Direction::RIGHT);
			}


			character->setLives(4);
			character->setHP(100);
			character->setPP(100);
			character->setName(obj.getString("name"));
			character->setLevel(obj.getString("level"));
			character->setKey(name);
			character->setAttackSize(obj.getObject("attack").getObject("size").getFloat("width"), obj.getObject("attack").getObject("size").getFloat("height"));
			character->setMidAttackY(obj.getObject("attack").getFloat("midPosition"));
			character->setLongAttackY(obj.getObject("attack").getFloat("longPosition"));
			character->setMidCost(obj.getObject("attack").getFloat("midCost"));
			character->setLongCost(obj.getObject("attack").getFloat("longCost"));
			character->setMidDamage(obj.getObject("attack").getFloat("midDamage"));
			character->setLongDamage(obj.getObject("attack").getFloat("longDamage"));
			
			std::string folder = "assets/characters/" + name + "/";
			game.getEngine()->getAudioEngine()->load("punch_" + name, folder + "punch", AUDIOTYPE::SOUND_EFFECT);
			game.getEngine()->getAudioEngine()->load("shoot_" + name, folder + "shoot", AUDIOTYPE::SOUND_EFFECT);

			loadSpriteMap(character, name, game, obj);

			game.getEngine()->getPhysicsEngine()->addBody(character, 40, 10);

			delete doc;

			return character;
		}

		void CharacterFactory::loadSpriteMap(Character *character, const std::string name, engine::GameBase &game, JSON::JSONObject &obj) {
			DrawEngine *drawEngine = game.getEngine()->getDrawEngine();
			float scale = obj.getFloat("scale");
			std::string folder = "assets/characters/" + name + "/";

			drawEngine->loadMap(character, MovableGameObject::State::WALKING, folder + "walking.sprite", obj.getArray("walking").getInt(0), obj.getArray("walking").getInt(1), scale);
			drawEngine->loadMap(character, MovableGameObject::State::IDLE, folder + "idle.sprite", obj.getArray("idle").getInt(0), obj.getArray("idle").getInt(1), scale);
			drawEngine->loadMap(character, MovableGameObject::State::JUMPING, folder + "jumping.sprite", obj.getArray("jumping").getInt(0), obj.getArray("jumping").getInt(1), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::JUMPINGLEFT, folder + "jump_end.sprite", obj.getArray("jumpEnd").getInt(0), obj.getArray("jumpEnd").getInt(1), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::JUMPINGRIGHT, folder + "jump_end.sprite", obj.getArray("jumpEnd").getInt(0), obj.getArray("jumpEnd").getInt(1), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::FALLING, folder + "falling.sprite", obj.getArray("falling").getInt(0), obj.getArray("falling").getInt(1), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::FALLINGLEFT, folder + "falling.sprite", obj.getArray("falling").getInt(0), obj.getArray("falling").getInt(1), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::FALLINGRIGHT, folder + "falling.sprite", obj.getArray("falling").getInt(0), obj.getArray("falling").getInt(1), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::KNOCKBACKLEFT, folder + "knockback.sprite", obj.getArray("knockback").getInt(0), obj.getArray("knockback").getInt(1), scale, Surface::AnimationType::ONCE);
			drawEngine->copyMap(character, MovableGameObject::State::KNOCKBACKLEFT, MovableGameObject::State::KNOCKBACKRIGHT);
			drawEngine->loadMap(character, MovableGameObject::State::KNEELING, folder + "kneeling.sprite", obj.getArray("kneeling").getInt(0), obj.getArray("kneeling").getInt(1), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::RESPAWN, folder + "jump_end.sprite", obj.getArray("jumpEnd").getInt(0), obj.getArray("jumpEnd").getInt(1), scale, Surface::AnimationType::HOLDLAST);

			drawEngine->loadMap(character, MovableGameObject::State::FORWARD_ROLL, folder + "forward_roll.sprite", obj.getArray("forwardRoll").getInt(0), obj.getArray("forwardRoll").getInt(1), scale, Surface::AnimationType::ONCE);
			drawEngine->copyMap(character, MovableGameObject::State::FORWARD_ROLL, MovableGameObject::State::IDLE | MovableGameObject::State::FORWARD_ROLL);
			drawEngine->copyMap(character, MovableGameObject::State::FORWARD_ROLL, MovableGameObject::State::WALKING | MovableGameObject::State::FORWARD_ROLL);
			drawEngine->copyMap(character, MovableGameObject::State::FORWARD_ROLL, MovableGameObject::State::JUMPING | MovableGameObject::State::FORWARD_ROLL);
			drawEngine->copyMap(character, MovableGameObject::State::FORWARD_ROLL, MovableGameObject::State::JUMPINGLEFT | MovableGameObject::State::FORWARD_ROLL);
			drawEngine->copyMap(character, MovableGameObject::State::FORWARD_ROLL, MovableGameObject::State::JUMPINGRIGHT | MovableGameObject::State::FORWARD_ROLL);
			drawEngine->copyMap(character, MovableGameObject::State::FORWARD_ROLL, MovableGameObject::State::FALLING | MovableGameObject::State::FORWARD_ROLL);
			drawEngine->copyMap(character, MovableGameObject::State::FORWARD_ROLL, MovableGameObject::State::FALLINGLEFT | MovableGameObject::State::FORWARD_ROLL);
			drawEngine->copyMap(character, MovableGameObject::State::FORWARD_ROLL, MovableGameObject::State::FALLINGRIGHT | MovableGameObject::State::FORWARD_ROLL);
			
			drawEngine->loadMap(character, MovableGameObject::State::IDLE | MovableGameObject::State::BLOCKING, folder + "blocking.sprite", obj.getArray("blocking").getInt(0), obj.getArray("blocking").getInt(1), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::IDLE | MovableGameObject::State::MIDRANGEATTACKBEGIN, folder + "mid_range_attack_begin.sprite", obj.getArray("midRangeAttackBegin").getInt(0), obj.getArray("midRangeAttackBegin").getInt(1), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::IDLE | MovableGameObject::State::MIDRANGEATTACK, folder + "mid_range_attack.sprite", obj.getArray("midRangeAttack").getInt(0), obj.getArray("midRangeAttack").getInt(1), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::IDLE | MovableGameObject::State::MIDRANGEATTACKEND, folder + "mid_range_attack_end.sprite", obj.getArray("midRangeAttackEnd").getInt(0), obj.getArray("midRangeAttackEnd").getInt(1), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACKBEGIN, folder + "mid_range_attack_walk_begin.sprite", obj.getArray("midRangeAttackWalkBegin").getInt(0), obj.getArray("midRangeAttackWalkBegin").getInt(1), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACK, folder + "mid_range_attack_walk.sprite", obj.getArray("midRangeAttackWalk").getInt(0), obj.getArray("midRangeAttackWalk").getInt(1), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACKEND, folder + "mid_range_attack_walk_end.sprite", obj.getArray("midRangeAttackWalkEnd").getInt(0), obj.getArray("midRangeAttackWalkEnd").getInt(1), scale, Surface::AnimationType::ONCE);

			drawEngine->loadMap(character, MovableGameObject::State::IDLE | MovableGameObject::State::LONGRANGEATTACKBEGIN, folder + "long_range_attack_begin.sprite", obj.getArray("longRangeAttackBegin").getInt(0), obj.getArray("longRangeAttackBegin").getInt(1), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::IDLE | MovableGameObject::State::LONGRANGEATTACK, folder + "long_range_attack.sprite", obj.getArray("longRangeAttack").getInt(0), obj.getArray("longRangeAttack").getInt(1), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::IDLE | MovableGameObject::State::LONGRANGEATTACKEND, folder + "long_range_attack_end.sprite", obj.getArray("longRangeAttackEnd").getInt(0), obj.getArray("longRangeAttackEnd").getInt(1), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACKBEGIN, folder + "long_range_attack_walk_begin.sprite", obj.getArray("longRangeAttackWalkBegin").getInt(0), obj.getArray("longRangeAttackWalkBegin").getInt(1), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACK, folder + "long_range_attack_walk.sprite", obj.getArray("longRangeAttackWalk").getInt(0), obj.getArray("longRangeAttackWalk").getInt(1), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACKEND, folder + "long_range_attack_walk_end.sprite", obj.getArray("longRangeAttackWalkEnd").getInt(0), obj.getArray("longRangeAttackWalkEnd").getInt(1), scale, Surface::AnimationType::ONCE);

			drawEngine->loadMap(character->getKey() + "_bullet", folder + "bullet.sprite", obj.getArray("bullet").getInt(0), obj.getArray("bullet").getInt(1));
		}
	}
}