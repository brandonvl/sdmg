//
//
//  @ Project : SDMG
//  @ File Name : LoadingSinglePlayerState.ccp
//  @ Date : 8-12-2014
//  @ Author : 42IN13SAd
//
//

#include "LoadingSinglePlayerState.h"
#include "PlayState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include <SDL.h>
#include "engine\physics\PhysicsEngine.h"
#include "model\Platform.h"
#include "model\MovablePlatform.h"
#include "model\Character.h"
#include "factories\CharacterFactory.h"
#include "engine\input\InputEngine.h"
#include "actions\Actions.h"
#include "engine\World.h"
#include "gamestates\MainMenuState.h"
#include "engine\audio\AudioEngine.h"
#include "TutorialState.h"
#include "helperclasses\HUD.h"
#include "engine\util\FileParser.h"
#include "lib\JSONParser.h"
#include "helperclasses\ConfigManager.h"
#include <array>
#include "engine\ai\EasyAIMachine.h"

#include "engine\util\FileManager.h"

#include <random>
// only for windows
// Advertisement ophalen
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

namespace sdmg {
	namespace gamestates {

		void LoadingSinglePlayerState::init(GameBase &game)
		{
			_game = &game;
			_game->getWorld()->clearWorld();
			
			// LoadingBar
			_loadingValue = 0;
			_marginInner = 3;
			_marginValue = 1;
			_totalWidth = 300;
			_totalHeight = 23;
			_loadingBarX = game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - _totalWidth / 2;
			_loadingBarY = 565;
		}

		void LoadingSinglePlayerState::cleanup(GameBase &game)
		{
			game.getEngine()->getDrawEngine()->unload("loading");
		}

		void LoadingSinglePlayerState::pause(GameBase &game)
		{
		}

		void LoadingSinglePlayerState::resume(GameBase &game)
		{
		}

		void LoadingSinglePlayerState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;
			while (SDL_PollEvent(&event));
		}

		void LoadingSinglePlayerState::update(GameBase &game, GameTime &gameTime)
		{
			if (_isLoaded)
			{
				PlayState &state = PlayState::getInstance();
				state.setSlotKeyBinding(_slotKeyInput, _keys);
				state.setHUDs(_huds);
				changeState(game, state);
			}
			if (_isError) {
				// Clean uppen
				changeState(game, MainMenuState::getInstance());
			}
		}

		void LoadingSinglePlayerState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("loading");

			if (_isAdvertisement)
				game.getEngine()->getDrawEngine()->draw("advertisement", _advertisementX, _advertisementY);

			game.getEngine()->getDrawEngine()->drawRectangle(Rectangle(_loadingBarX, _loadingBarY, _totalWidth, _totalHeight), 200, 200, 200);
			game.getEngine()->getDrawEngine()->drawRectangle(Rectangle(_loadingBarX + _marginInner, _loadingBarY + _marginInner, _totalWidth - (_marginInner * 2), _totalHeight - (_marginInner * 2)), 255, 255, 255);
			game.getEngine()->getDrawEngine()->drawRectangle(Rectangle(_loadingBarX + _marginInner + _marginValue, _loadingBarY + _marginInner + _marginValue, _loadingValue, _totalHeight - (_marginInner * 2) - (_marginValue * 2)), 50, 50, 50);

			game.getEngine()->getDrawEngine()->render();
		}

		void LoadingSinglePlayerState::load() {
			int maxLoadingValue = _totalWidth - (_marginInner * 2) - (_marginValue * 2);
			_loadingStep = maxLoadingValue / 3;

			loadAdvertisement();
			loadCharacters();
			loadKeybindings();

			_loadingValue = maxLoadingValue;

			_isLoaded = true;
			clearEventQueue();
		}

		void LoadingSinglePlayerState::clearEventQueue() {
			SDL_Event event;
			while (SDL_PollEvent(&event));
		}

		void LoadingSinglePlayerState::loadLevel(std::string level) {

			_game->getStateManager()->draw();

			JSON::JSONDocument *doc = JSON::JSONDocument::fromFile("assets/levels/" + level + "/data");
			JSON::JSONObject &levelObj = doc->getRootObject();

			PhysicsEngine *pe = _game->getEngine()->getPhysicsEngine();
			DrawEngine *de = _game->getEngine()->getDrawEngine();

			pe->setWorldGravity(levelObj.getObject("gravity").getFloat("left"), levelObj.getObject("gravity").getFloat("down"));

			JSON::JSONArray &platformArr = levelObj.getArray("platforms");

			if (platformArr.size() > 0) {
				int platformStep = (_loadingStep / 3) / platformArr.size();

				for (int i = 0; i < platformArr.size(); i++) {
					JSON::JSONObject &platformObj = platformArr.getObject(i);

					Platform *platform = new model::Platform();
					platform->setSize(platformObj.getObject("size").getFloat("width"), platformObj.getObject("size").getFloat("height"));
					platform->setLocation(platformObj.getObject("location").getFloat("x"), platformObj.getObject("location").getFloat("y"));
					pe->addBody(platform, platformObj.getObject("bodyPadding").getFloat("x"), platformObj.getObject("bodyPadding").getFloat("y"));
					if (platformObj.exists("canMoveThroughIt"))
						platform->setCanMoveThroughIt(false);
					else
						platform->setCanMoveThroughIt(true);

					_game->getWorld()->addPlatform(platform);
					if (platformObj.exists("image"))
						de->load(platform, "assets/levels/" + level + "/" + platformObj.getString("image"));

					_loadingValue += platformStep;
					_game->getStateManager()->draw();
				}
			}
			else {
				_loadingValue += _loadingStep / 3;
			}

			de->load("overlay", "assets/levels/" + level + "/overlay");
			de->load("background", "assets/levels/" + level + "/background");
			_game->getEngine()->getAudioEngine()->load("bgm", "assets/levels/" + level + "/bgm.mp3", AUDIOTYPE::MUSIC);

			std::vector<MovableGameObject*> players = _game->getWorld()->getPlayers();
			
			for (size_t i = 0; i < players.size(); i++)
			{
				JSON::JSONObject &posObj = levelObj.getArray("startingPositions").getObject(i);

				float xPos = posObj.getFloat("x"), yPos = posObj.getFloat("y");

				players[i]->getBody()->SetTransform(b2Vec2(xPos / 20.0f, yPos / 20.0f), players[i]->getBody()->GetAngle());
				players[i]->setSpawnLocation(xPos, yPos);
			}

			loadBulletBobs(levelObj.getArray("bobs"));

			// Load fps text
			de->loadDynamicText("fps", { 255, 255, 255 }, "arial", 18);
			de->loadDynamicText("speed", { 255, 255, 255 }, "arial", 18);
			delete doc;
		}

		void LoadingSinglePlayerState::unloadAll()
		{
			delete _playerName;
			_playerName = nullptr;
			delete _levelName;
			_levelName = nullptr;

			if (_enemies) {
				for (auto it : *_enemies) {
					delete it;
				}
				_enemies->clear();
			}
			delete _enemies;
			_enemies = nullptr;

			delete _slotKeyInput;
			_slotKeyInput = nullptr;
			delete _keys;
			_keys = nullptr;
		}

		bool LoadingSinglePlayerState::hasFinished()
		{
			return _enemies->size() <= 1;
		}

		std::string LoadingSinglePlayerState::getLevelName()
		{
			return *_levelName;
		}

		std::string LoadingSinglePlayerState::getPlayerName()
		{
			return *_playerName;
		}

		void LoadingSinglePlayerState::setPlayerName(std::string playerName)
		{
			_playerName = new std::string(playerName);
			setEnemies();
		}

		void LoadingSinglePlayerState::setSlotKeyBinding(std::map<std::string, int> *input, std::vector<std::string> *keys)
		{
			_slotKeyInput = new std::map<std::string, int>(*input);
			_keys = new std::vector<std::string>(*keys);
		}

		std::string LoadingSinglePlayerState::getSlotKeyInput(int slot) {
			std::string key = "slot_key_" + std::to_string(slot);
			auto it = _slotKeyInput->find(key);
			if (it != _slotKeyInput->end())
				return _keys->at(it->second);
			else
				return "";
		}

		void LoadingSinglePlayerState::setEnemies()
		{
			if (_enemies) {
				for (auto it : *_enemies) {
					delete it;
				}
				_enemies->clear();
			}
			delete _enemies;

			_enemies = new std::vector<std::string*>();
			std::vector<std::string> enemies = util::FileManager::getInstance().getFolders("assets/characters/");

			for (std::string e : enemies)
				if (e != *_playerName)
					_enemies->push_back(new std::string(e));

			std::string boss = ConfigManager::getInstance().getUnlockableCharacterName(*_playerName);

			for (size_t i = 0; i < enemies.size(); i++)
			{
				if (*(*_enemies)[i] == boss)
				{
					delete (*_enemies)[i];
					(*_enemies).erase((*_enemies).begin() + i);
					break;
				}
			}
			_enemies->push_back(new std::string(boss));
			
			_removeFirstEnemy = false;
		}

		void LoadingSinglePlayerState::loadNextFight()
		{
			delete _levelName;
			_levelName = nullptr;

			_isLoaded = false;
			_isError = false;
			_isAdvertisement = false;

			_game->getEngine()->getAudioEngine()->load("hurt_punch", "assets\\sounds\\effects\\hurt_punch.ogg", AUDIOTYPE::SOUND_EFFECT);
			_game->getEngine()->getAudioEngine()->load("hurt_shoot", "assets\\sounds\\effects\\hurt_shoot.ogg", AUDIOTYPE::SOUND_EFFECT);
			_game->getEngine()->getDrawEngine()->load("loading", "assets\\screens\\loadingscreen");
			_game->getStateManager()->draw();

			if (_removeFirstEnemy)
			{
				delete (*_enemies)[0];
				_enemies->erase(_enemies->begin());
			}
			_removeFirstEnemy = true;

			load();
			_game->getEngine()->getAudioEngine()->unload("main_menu_bgm");

			_game->getEngine()->getPhysicsEngine()->resume();

			_game->getStateManager()->update();
		}

		void LoadingSinglePlayerState::loadCharacters() {
			std::string loadCharacters[] = { *_playerName, *(*_enemies)[0] }, level;
			std::vector<Character*> characters(sizeof(loadCharacters));

			int characterStep = 0;
			if (characterStep <= 0)
				characterStep = (_loadingStep / 3);
			else
				characterStep = (_loadingStep / 3) / (loadCharacters->size() + 1);

			for (int i = 0; i < 2; i++) {

				_game->getStateManager()->draw();

				int retries = 0;
				do{
					if (i==0)
						characters[i] = factories::CharacterFactory::create(loadCharacters[i], *_game, 100.0f, -100.0f);
					else
						characters[i] = factories::CharacterFactory::create(loadCharacters[i], *_game, 1100.0f, -100.0f);
					_game->getWorld()->addPlayer(characters[i]);
					level = characters[i]->getLevel();
					if (retries++ > 10){
						_isError = true;
						return;
					}
				} while (characters[i] == nullptr);

				_loadingValue += characterStep;
				_game->getStateManager()->draw();
			}

			characters[1]->setDirection(MovableGameObject::Direction::LEFT);
			characters[1]->setSpawnDirection(MovableGameObject::Direction::LEFT);

			engine::ai::AIMachine *machine = new engine::ai::EasyAIMachine(*characters[1], *characters[0]);
			characters[1]->setAI(*machine);
			characters[1]->setLives(1);

			_game->getStateManager()->draw();

			// Create a HUD for each player
			_huds = new std::vector<helperclasses::HUD*>();

			HUD *hudPanda = new HUD(*characters[0], 10);
			_huds->push_back(hudPanda);

			HUD *hudNivek = new HUD(*characters[1], _game->getEngine()->getDrawEngine()->getWindowWidth() - 230 - 10);
			_huds->push_back(hudNivek);

			_loadingValue += characterStep;

			_levelName = new std::string(level);
			loadLevel(level);
		}

		void LoadingSinglePlayerState::loadBulletBobs(JSON::JSONArray &bobs) {
			_game->getStateManager()->draw();

			int bobStep = 0;
			if (bobStep <= 0)
				_loadingValue += (_loadingStep / 3);
			else
				bobStep = (_loadingStep / 3) / bobs.size();

			for (int i = 0; i < bobs.size(); i++) {
				JSON::JSONObject &bobObj = bobs.getObject(i);

				MovablePlatform *platform = new MovablePlatform();
				platform->setSize(bobObj.getObject("size").getFloat("width"), bobObj.getObject("size").getFloat("height"));
				platform->setLocation(bobObj.getObject("location").getFloat("x"), bobObj.getObject("location").getFloat("y"));
				platform->setStartLocation(b2Vec2(bobObj.getObject("location").getFloat("x"), bobObj.getObject("location").getFloat("y")));
				platform->setEndLocation(b2Vec2(bobObj.getObject("endLocation").getFloat("x"), bobObj.getObject("endLocation").getFloat("y")));
				platform->setDirection(static_cast<MovableGameObject::Direction>((int)bobObj.getFloat("direction")));
				platform->setSpeed(bobObj.getObject("speed").getFloat("horizontal"), bobObj.getObject("speed").getFloat("vertical"));
				platform->setDamageOnImpact(100);

				_game->getWorld()->addPlatform(platform);
				_game->getEngine()->getPhysicsEngine()->addKinematicBody(platform);
				_game->getEngine()->getDrawEngine()->loadMap(platform, MovableGameObject::State::IDLE, R"(assets\levels\level2\bullet.sprite)", 1097, 494, 0.1);

				_loadingValue += bobStep;
				_game->getStateManager()->draw();
			}
		}

		void LoadingSinglePlayerState::loadKeybindings() {
			_game->getStateManager()->draw();

			try{
				ConfigManager &manager = ConfigManager::getInstance();
				//InputDeviceBinding *binding = new InputDeviceBinding();

				const std::vector<MovableGameObject*> players = _game->getWorld()->getPlayers();

				//  int controlStep = (_loadingStep / 3) / players.size();
				int controlStep = 0;
				if (controlStep <= 0)
					_loadingValue += (_loadingStep / 3);
				else
					controlStep = (_loadingStep / 3) / players.size() - 1;

				_game->getEngine()->getInputEngine()->clearBindings();

				int playerCharacterID = 0;
				
				std::string keyBinding = getSlotKeyInput(playerCharacterID);
				InputDeviceBinding *binding = _game->getEngine()->getInputEngine()->createBinding(keyBinding);
				int deviceIndexInFile = manager.getDeviceIndex(keyBinding);

				Character *character = static_cast<Character*>(players[playerCharacterID]);
				binding->setKeyBinding(manager.getKey(deviceIndexInFile, "walkRight"), new actions::RightWalkAction(character));
				binding->setKeyBinding(manager.getKey(deviceIndexInFile, "walkLeft"), new actions::LeftWalkAction(character));
				binding->setKeyBinding(manager.getKey(deviceIndexInFile, "jump"), new actions::JumpAction(character));
				binding->setKeyBinding(manager.getKey(deviceIndexInFile, "roll"), new actions::RollAction(character));
				binding->setKeyBinding(manager.getKey(deviceIndexInFile, "midrange"), new actions::MidRangeAttackAction(character));
				binding->setKeyBinding(manager.getKey(deviceIndexInFile, "longrange"), new actions::LongRangeAttackAction(character));
				binding->setKeyBinding(manager.getKey(deviceIndexInFile, "block"), new actions::BlockAction(character));
				_game->getEngine()->getInputEngine()->setDeviceBinding(keyBinding, binding);

				_loadingValue += controlStep;
				_game->getStateManager()->draw();

				//_game->getEngine()->getInputEngine()->setDeviceBinding("keyboard", binding);
			}
			catch (...)
			{
				std::cout << "Cannot load keyboard bindings.";
			}
		}

		void LoadingSinglePlayerState::loadAdvertisement()
		{
			_game->getStateManager()->draw();

			std::vector<std::string> advertismentList = util::FileManager::getInstance().getFiles("assets/advertisements/");

			if (advertismentList.size() > 0)
			{
				_isAdvertisement = true;

				std::random_device dev;
				std::default_random_engine dre(dev());
				std::uniform_int_distribution<int> randomAdvertisement(0, advertismentList.size() - 1);

				_game->getEngine()->getDrawEngine()->load("advertisement", "assets\\advertisements\\" + advertismentList[randomAdvertisement(dre)]);

				const std::array<int, 2> size = _game->getEngine()->getDrawEngine()->getImageSize("advertisement");
				_advertisementX = _game->getEngine()->getDrawEngine()->getWindowWidth() - size[0] - 10;
				_advertisementY = _game->getEngine()->getDrawEngine()->getWindowHeight() - size[1] - 10;

			}

			_loadingValue += _loadingStep;
		}
	}
}