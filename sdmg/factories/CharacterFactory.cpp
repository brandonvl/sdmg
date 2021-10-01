#include "CharacterFactory.h"
#include "model\Character.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\GameBase.h"
#include "engine\Engine.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\World.h"
#include "model\Platform.h"
#include "engine\audio\AudioEngine.h"
#include "engine/util/FileManager.h"

namespace sdmg {
	namespace factories {
		Character *CharacterFactory::create(const std::string name, engine::GameBase &game,const float &xPosition, const float &yPosition) 
		{
			std::string folder = "assets/characters/" + name + "/";
			auto json = engine::util::FileManager::getInstance().loadFileContentsAsJson(folder + "/data");
			auto jsonSizeObject = json["size"].get<nlohmann::json>();
			auto jsonSpeedObject = json["speed"].get<nlohmann::json>();

			Character *character = new Character();
			character->setSize(jsonSizeObject["width"].get<float>(), jsonSizeObject["height"].get<float>());
			character->setSpeed(jsonSpeedObject["horizontal"].get<float>(), jsonSpeedObject["vertical"].get<float>());
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

			auto jsonAttackObject = json["attack"].get<nlohmann::json>();

			character->setLives(4);
			character->setHP(100);
			character->setPP(100);
			character->setName(json["name"].get<std::string>());
			character->setLevel(json["level"].get<std::string>());
			character->setKey(name);
			character->setAttackSize(jsonAttackObject["size"]["width"].get<float>(), jsonAttackObject["size"]["height"].get<float>());
			character->setMidAttackY(jsonAttackObject["midPosition"].get<float>());
			character->setLongAttackY(jsonAttackObject["longPosition"].get<float>());
			character->setMidCost(jsonAttackObject["midCost"].get<float>());
			character->setLongCost(jsonAttackObject["longCost"].get<float>());
			character->setMidDamage(jsonAttackObject["midDamage"].get<float>());
			character->setLongDamage(jsonAttackObject["longDamage"].get<float>());
			character->setMultiplierOnHit(jsonAttackObject["damageMultiplier"].get<float>()); 
			
			
			game.getEngine()->getAudioEngine()->load("punch_" + name, folder + "punch", AUDIOTYPE::SOUND_EFFECT);
			game.getEngine()->getAudioEngine()->load("shoot_" + name, folder + "shoot", AUDIOTYPE::SOUND_EFFECT);

			loadSpriteMap(character, name, game, json);

			game.getEngine()->getPhysicsEngine()->addBody(character, 40, 10);
			return character;
		}

		void CharacterFactory::loadSpriteMap(Character *character, const std::string name, engine::GameBase &game, nlohmann::json& json) {
			DrawEngine *drawEngine = game.getEngine()->getDrawEngine();
			float scale = json["scale"].get<float>();
			std::string folder = "assets/characters/" + name + "/";

			drawEngine->loadMap(character, MovableGameObject::State::WALKING, folder + "walking.sprite", json["walking"][0].get<int>(), json["walking"][1].get<int>(), scale);
			drawEngine->loadMap(character, MovableGameObject::State::IDLE, folder + "idle.sprite", json["idle"][0].get<int>(), json["idle"][1].get<int>(), scale);
			drawEngine->loadMap(character, MovableGameObject::State::JUMPING, folder + "jumping.sprite", json["jumping"][0].get<int>(), json["jumping"][1].get<int>(), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::JUMPINGLEFT, folder + "jump_end.sprite", json["jumpEnd"][0].get<int>(), json["jumpEnd"][1].get<int>(), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::JUMPINGRIGHT, folder + "jump_end.sprite", json["jumpEnd"][0].get<int>(), json["jumpEnd"][1].get<int>(), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::FALLING, folder + "falling.sprite", json["falling"][0].get<int>(), json["falling"][1].get<int>(), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::FALLINGLEFT, folder + "falling.sprite", json["falling"][0].get<int>(), json["falling"][1].get<int>(), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::FALLINGRIGHT, folder + "falling.sprite", json["falling"][0].get<int>(), json["falling"][1].get<int>(), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::KNOCKBACKLEFT, folder + "knockback.sprite", json["knockback"][0].get<int>(), json["knockback"][1].get<int>(), scale, Surface::AnimationType::ONCE);
			drawEngine->copyMap(character, MovableGameObject::State::KNOCKBACKLEFT, MovableGameObject::State::KNOCKBACKRIGHT);
			drawEngine->loadMap(character, MovableGameObject::State::KNEELING, folder + "kneeling.sprite", json["kneeling"][0].get<int>(), json["kneeling"][1].get<int>(), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::RESPAWN, folder + "jump_end.sprite", json["jumpEnd"][0].get<int>(), json["jumpEnd"][1].get<int>(), scale, Surface::AnimationType::HOLDLAST);

			drawEngine->loadMap(character, MovableGameObject::State::FORWARD_ROLL, folder + "forward_roll.sprite", json["forwardRoll"][0].get<int>(), json["forwardRoll"][1].get<int>(), scale, Surface::AnimationType::ONCE);
			drawEngine->copyMap(character, MovableGameObject::State::FORWARD_ROLL, MovableGameObject::State::IDLE | MovableGameObject::State::FORWARD_ROLL);
			drawEngine->copyMap(character, MovableGameObject::State::FORWARD_ROLL, MovableGameObject::State::WALKING | MovableGameObject::State::FORWARD_ROLL);
			drawEngine->copyMap(character, MovableGameObject::State::FORWARD_ROLL, MovableGameObject::State::JUMPING | MovableGameObject::State::FORWARD_ROLL);
			drawEngine->copyMap(character, MovableGameObject::State::FORWARD_ROLL, MovableGameObject::State::JUMPINGLEFT | MovableGameObject::State::FORWARD_ROLL);
			drawEngine->copyMap(character, MovableGameObject::State::FORWARD_ROLL, MovableGameObject::State::JUMPINGRIGHT | MovableGameObject::State::FORWARD_ROLL);
			drawEngine->copyMap(character, MovableGameObject::State::FORWARD_ROLL, MovableGameObject::State::FALLING | MovableGameObject::State::FORWARD_ROLL);
			drawEngine->copyMap(character, MovableGameObject::State::FORWARD_ROLL, MovableGameObject::State::FALLINGLEFT | MovableGameObject::State::FORWARD_ROLL);
			drawEngine->copyMap(character, MovableGameObject::State::FORWARD_ROLL, MovableGameObject::State::FALLINGRIGHT | MovableGameObject::State::FORWARD_ROLL);
			
			drawEngine->loadMap(character, MovableGameObject::State::IDLE | MovableGameObject::State::BLOCKING, folder + "blocking.sprite", json["blocking"][0].get<int>(), json["blocking"][1].get<int>(), scale, Surface::AnimationType::HOLDLAST);
			drawEngine->loadMap(character, MovableGameObject::State::IDLE | MovableGameObject::State::MIDRANGEATTACKBEGIN, folder + "mid_range_attack_begin.sprite", json["midRangeAttackBegin"][0].get<int>(), json["midRangeAttackBegin"][1].get<int>(), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::IDLE | MovableGameObject::State::MIDRANGEATTACK, folder + "mid_range_attack.sprite", json["midRangeAttack"][0].get<int>(), json["midRangeAttack"][1].get<int>(), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::IDLE | MovableGameObject::State::MIDRANGEATTACKEND, folder + "mid_range_attack_end.sprite", json["midRangeAttackEnd"][0].get<int>(), json["midRangeAttackEnd"][1].get<int>(), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACKBEGIN, folder + "mid_range_attack_walk_begin.sprite", json["midRangeAttackWalkBegin"][0].get<int>(), json["midRangeAttackWalkBegin"][1].get<int>(), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACK, folder + "mid_range_attack_walk.sprite", json["midRangeAttackWalk"][0].get<int>(), json["midRangeAttackWalk"][1].get<int>(), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::WALKING | MovableGameObject::State::MIDRANGEATTACKEND, folder + "mid_range_attack_walk_end.sprite", json["midRangeAttackWalkEnd"][0].get<int>(), json["midRangeAttackWalkEnd"][1].get<int>(), scale, Surface::AnimationType::ONCE);

			drawEngine->loadMap(character, MovableGameObject::State::IDLE | MovableGameObject::State::LONGRANGEATTACKBEGIN, folder + "long_range_attack_begin.sprite", json["longRangeAttackBegin"][0].get<int>(), json["longRangeAttackBegin"][1].get<int>(), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::IDLE | MovableGameObject::State::LONGRANGEATTACK, folder + "long_range_attack.sprite", json["longRangeAttack"][0].get<int>(), json["longRangeAttack"][1].get<int>(), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::IDLE | MovableGameObject::State::LONGRANGEATTACKEND, folder + "long_range_attack_end.sprite", json["longRangeAttackEnd"][0].get<int>(), json["longRangeAttackEnd"][1].get<int>(), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACKBEGIN, folder + "long_range_attack_walk_begin.sprite", json["longRangeAttackWalkBegin"][0].get<int>(), json["longRangeAttackWalkBegin"][1].get<int>(), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACK, folder + "long_range_attack_walk.sprite", json["longRangeAttackWalk"][0].get<int>(), json["longRangeAttackWalk"][1].get<int>(), scale, Surface::AnimationType::ONCE);
			drawEngine->loadMap(character, MovableGameObject::State::WALKING | MovableGameObject::State::LONGRANGEATTACKEND, folder + "long_range_attack_walk_end.sprite", json["longRangeAttackWalkEnd"][0].get<int>(), json["longRangeAttackWalkEnd"][1].get<int>(), scale, Surface::AnimationType::ONCE);

			drawEngine->loadMap(character->getKey() + "_bullet", folder + "bullet.sprite", json["bullet"][0].get<int>(), json["bullet"][1].get<int>());
		}
	}
}