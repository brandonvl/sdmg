//
//
//  @ Project : SDMG
//  @ File Name : LoadingSurvivalState.cpp
//  @ Date : 10-12-2014
//  @ Author : 42IN13SAd
//
//

#include "LoadingSurvivalState.h"
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
#include "helperclasses\ConfigManager.h"
#include "engine\ai\EasyAIMachine.h"
#include <array>
#include <random>

#include "engine\util\FileManager.h"

namespace sdmg
{
	namespace gamestates
	{

		void LoadingSurvivalState::init(GameBase& game)
		{
			_game = &game;
			_game->getWorld()->clearWorld();

			_levelName = new std::string("level1");

			// LoadingBar
			_loadingValue = 0;
			_marginInner = 3;
			_marginValue = 1;
			_totalWidth = 300;
			_totalHeight = 23;
			_loadingBarX = game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - _totalWidth / 2;
			_loadingBarY = 565;

			_isLoaded = false;
			_isError = false;
			_isAdvertisement = false;

			game.getEngine()->getDrawEngine()->load("loading", "assets\\screens\\loadingscreen");

			_game->getStateManager()->draw();

			game.getEngine()->getAudioEngine()->load("hurt_punch", "assets\\sounds\\effects\\hurt_punch.ogg", AUDIOTYPE::SOUND_EFFECT);
			game.getEngine()->getAudioEngine()->load("hurt_shoot", "assets\\sounds\\effects\\hurt_shoot.ogg", AUDIOTYPE::SOUND_EFFECT);
			load();
			game.getEngine()->getAudioEngine()->unload("main_menu_bgm");

			_game->getEngine()->getPhysicsEngine()->resume();

			_game->getStateManager()->update();
		}

		std::string LoadingSurvivalState::getLevelName()
		{
			return *_levelName;
		}

		std::string LoadingSurvivalState::getPlayerName()
		{
			return *_playerName;
		}

		void LoadingSurvivalState::setPlayerName(std::string playerName)
		{
			_playerName = new std::string(playerName);
			setEnemies();
		}

		void LoadingSurvivalState::setEnemies()
		{
			if (_enemyNames)
			{
				for (auto it : *_enemyNames)
				{
					delete it;
				}
				_enemyNames->clear();
			}
			delete _enemyNames;

			_enemyNames = new std::vector<std::string*>();
			std::vector<std::string> enemies = util::FileManager::getInstance().getFolders("assets/characters/");

			for (std::string e : enemies)
				if (e != *_playerName)
					_enemyNames->push_back(new std::string(e));
		}

		void LoadingSurvivalState::cleanup(GameBase& game)
		{
			delete _levelName;
			delete _playerName;


			delete _slotKeyInput;
			_slotKeyInput = nullptr;
			delete _keys;
			_keys = nullptr;

			if (_enemyNames)
			{
				for (auto it : *_enemyNames)
				{
					delete it;
				}
				_enemyNames->clear();
			}
			delete _enemyNames;
			_enemyNames = nullptr;

			game.getEngine()->getDrawEngine()->unload("loading");
			game.getEngine()->getDrawEngine()->unloadText("progress");
		}

		void LoadingSurvivalState::pause(GameBase& game)
		{
		}

		void LoadingSurvivalState::resume(GameBase& game)
		{
		}

		void LoadingSurvivalState::handleEvents(GameBase& game, GameTime& gameTime)
		{
			SDL_Event event;
			if (SDL_PollEvent(&event));
		}

		void LoadingSurvivalState::update(GameBase& game, GameTime& gameTime)
		{
			if (_isLoaded)
			{
				PlayState& state = (_isTutorial ? TutorialState::getInstance() : PlayState::getInstance());
				state.setHUDs(_huds);
				state.setSlotKeyBinding(_slotKeyInput, _keys);
				changeState(game, state);
			}
			if (_isError)
			{
				// Clean uppen
				changeState(game, MainMenuState::getInstance());
			}
		}

		void LoadingSurvivalState::draw(GameBase& game, GameTime& gameTime)
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

		void LoadingSurvivalState::load()
		{
			int maxLoadingValue = _totalWidth - (_marginInner * 2) - (_marginValue * 2);
			_loadingStep = maxLoadingValue / 3;

			loadAdvertisement();
			loadLevel();
			loadKeybindings();

			_loadingValue = maxLoadingValue;

			_isLoaded = true;
			clearEventQueue();
		}

		void LoadingSurvivalState::clearEventQueue()
		{
			SDL_Event event;
			while (SDL_PollEvent(&event));
		}

		void LoadingSurvivalState::loadLevel()
		{
			_game->getStateManager()->draw();

			std::string folder = "assets/levels/" + (*_levelName);
			auto json = engine::util::FileManager::getInstance().loadFileContentsAsJson(folder + "/data");
			auto& jsonGravityObject = json["gravity"];
			PhysicsEngine* pe = _game->getEngine()->getPhysicsEngine();
			DrawEngine* de = _game->getEngine()->getDrawEngine();

			pe->setWorldGravity(jsonGravityObject["left"].get<float>(), jsonGravityObject["down"].get<float>());

			auto& jsonPlatforms = json["platforms"];

			if (jsonPlatforms.size() > 0)
			{
				int platformStep = (_loadingStep / 3) / jsonPlatforms.size();

				for (auto& jsonPlatform : jsonPlatforms)
				{
					Platform* platform = new model::Platform();
					platform->setSize(jsonPlatform["size"]["width"].get<float>(), jsonPlatform["size"]["height"].get<float>());
					platform->setLocation(jsonPlatform["location"]["x"].get<float>(), jsonPlatform["location"]["y"].get<float>());
					pe->addBody(platform, jsonPlatform["bodyPadding"]["x"].get<float>(), jsonPlatform["bodyPadding"]["y"].get<float>());

					if (jsonPlatform.contains("canMoveThroughIt"))
						platform->setCanMoveThroughIt(false);
					else
						platform->setCanMoveThroughIt(true);

					_game->getWorld()->addPlatform(platform);

					if (jsonPlatform.contains("image"))
						de->load(platform, folder + "/" + jsonPlatform["image"].get<std::string>());

					_loadingValue += platformStep;
					_game->getStateManager()->draw();
				}
			}
			else
			{
				_loadingValue += _loadingStep / 3;
			}

			de->load("overlay", "assets/levels/" + (*_levelName) + "/overlay");
			de->load("background", "assets/levels/" + (*_levelName) + "/background");
			_game->getEngine()->getAudioEngine()->load("bgm", folder + "/bgm.mp3", AUDIOTYPE::MUSIC);

			loadCharacters(json["startingPositions"]);

			loadBulletBobs(json["bobs"]);

			de->loadDynamicText("fps", { 255, 255, 255 }, "arial", 18);
			de->loadDynamicText("speed", { 255, 255, 255 }, "arial", 18);
		}


		void LoadingSurvivalState::loadCharacters(nlohmann::json& startingPositions)
		{
			Character* player = nullptr;
			_enemies = new std::vector<model::Character*>((*_enemyNames).size());

			int characterStep = (_loadingStep / 3) / ((*_enemyNames).size() + 1);

			_game->getStateManager()->draw();

			int retries = 0;
			do
			{
				auto& jsonCharacterStartingPosition = startingPositions[0];
				player = factories::CharacterFactory::create(*_playerName, *_game, jsonCharacterStartingPosition["x"].get<float>(), jsonCharacterStartingPosition["y"].get<float>());
				_game->getWorld()->addPlayer(player);
				if (retries++ > 10)
				{
					_isError = true;
					return;
				}
			} while (player == nullptr);

			_loadingValue += characterStep;

			for (int i = 0; i < (*_enemyNames).size(); i++)
			{

				_game->getStateManager()->draw();

				retries = 0;
				do
				{
					auto& jsonEnemyStartingPosition = startingPositions[1];
					(*_enemies)[i] = factories::CharacterFactory::create(*(*_enemyNames)[i], *_game, jsonEnemyStartingPosition["x"].get<float>(), jsonEnemyStartingPosition["y"].get<float>());
					(*_enemies)[i]->setDirection(MovableGameObject::Direction::LEFT);
					(*_enemies)[i]->setSpawnDirection(MovableGameObject::Direction::LEFT);
					if (i > 0)
						(*_enemies)[i]->getBody()->SetActive(false);
					_game->getWorld()->addPlayer((*_enemies)[i]);

					if (retries++ > 10)
					{
						_isError = true;
						return;
					}
				} while ((*_enemies)[i] == nullptr);

				_loadingValue += characterStep;
			}

			PlayState::getInstance().setPlayer(player);
			PlayState::getInstance().setEnemies(_enemies);

			_game->getWorld()->clearAliveList();

			_game->getWorld()->addAlive(player);
			_game->getWorld()->addAlive((*_enemies)[0]);

			_game->getStateManager()->draw();

			// Create a HUD for each player
			_huds = new std::vector<helperclasses::HUD*>();

			HUD* hudPanda = new HUD(*player, 10);
			_huds->push_back(hudPanda);

			HUD* hudNivek = new HUD(*(*_enemies)[0], _game->getEngine()->getDrawEngine()->getWindowWidth() - 230 - 10);
			_huds->push_back(hudNivek);

			_loadingValue += characterStep;
		}

		void LoadingSurvivalState::loadBulletBobs(nlohmann::json& bobs)
		{
			_game->getStateManager()->draw();

			int bobStep = 0;
			if (bobStep <= 0)
				_loadingValue += (_loadingStep / 3);
			else
				bobStep = (_loadingStep / 3) / bobs.size();

			for (int i = 0; i < bobs.size(); i++)
			{
				auto& jsonBobObject = bobs[i];

				MovablePlatform* platform = new MovablePlatform();
				platform->setSize(jsonBobObject["size"]["width"].get<float>(), jsonBobObject["size"]["height"].get<float>());
				platform->setLocation(jsonBobObject["location"]["x"].get<float>(), jsonBobObject["location"]["y"].get<float>());
				platform->setStartLocation(b2Vec2(jsonBobObject["location"]["x"].get<float>(), jsonBobObject["location"]["y"].get<float>()));
				platform->setEndLocation(b2Vec2(jsonBobObject["endLocation"]["x"].get<float>(), jsonBobObject["endLocation"]["y"].get<float>()));
				platform->setDirection(static_cast<MovableGameObject::Direction>((int)jsonBobObject["direction"].get<float>()));
				platform->setSpeed(jsonBobObject["speed"]["horizontal"].get<float>(), jsonBobObject["speed"]["vertical"].get<float>());
				platform->setDamageOnImpact(100);

				_game->getWorld()->addPlatform(platform);
				_game->getEngine()->getPhysicsEngine()->addKinematicBody(platform);
				_game->getEngine()->getDrawEngine()->loadMap(platform, MovableGameObject::State::IDLE, R"(assets\levels\level2\bullet.sprite)", 1097, 494, 0.1);

				_loadingValue += bobStep;
				_game->getStateManager()->draw();
			}
		}

		std::string LoadingSurvivalState::getSlotKeyInput(int slot)
		{
			std::string key = "slot_key_" + std::to_string(slot);
			auto it = _slotKeyInput->find(key);
			if (it != _slotKeyInput->end())
				return _keys->at(it->second);
			else
				return "";
		}

		void LoadingSurvivalState::loadKeybindings()
		{

			_game->getStateManager()->draw();

			try
			{
				ConfigManager& manager = ConfigManager::getInstance();
				int playerCharacterID = 0;

				int controlStep = _loadingStep;

				_game->getEngine()->getInputEngine()->clearBindings();

				std::string keyBinding = getSlotKeyInput(playerCharacterID);
				InputDeviceBinding* binding = _game->getEngine()->getInputEngine()->createBinding(keyBinding);
				int deviceIndexInFile = manager.getDeviceIndex(keyBinding);
				const std::vector<MovableGameObject*> players = _game->getWorld()->getPlayers();

				Character* character = static_cast<Character*>(players[playerCharacterID]);
				binding->setKeyBinding(manager.getKey(deviceIndexInFile, "walkRight"), new actions::RightWalkAction(character));
				binding->setKeyBinding(manager.getKey(deviceIndexInFile, "walkLeft"), new actions::LeftWalkAction(character));
				binding->setKeyBinding(manager.getKey(deviceIndexInFile, "jump"), new actions::JumpAction(character));
				binding->setKeyBinding(manager.getKey(deviceIndexInFile, "roll"), new actions::RollAction(character));
				binding->setKeyBinding(manager.getKey(deviceIndexInFile, "midrange"), new actions::MidRangeAttackAction(character));
				binding->setKeyBinding(manager.getKey(deviceIndexInFile, "longrange"), new actions::LongRangeAttackAction(character));
				binding->setKeyBinding(manager.getKey(deviceIndexInFile, "block"), new actions::BlockAction(character));

				for (size_t i = 1; i < players.size(); i++)
				{
					engine::ai::AIMachine* machine = new engine::ai::EasyAIMachine(*static_cast<Character*>(players[i]), *static_cast<Character*>(players[0]));
					static_cast<Character*>(players[i])->setAI(*machine);

					if (i > 1)
						machine->pause();
				}

				_loadingValue += controlStep;
				_game->getStateManager()->draw();

				_game->getEngine()->getInputEngine()->setDeviceBinding(keyBinding, binding);
			}
			catch (...)
			{
				std::cout << "Cannot load bindings.";
			}
		}

		void LoadingSurvivalState::setSlotKeyBinding(std::map<std::string, int>* input, std::vector<std::string>* keys)
		{
			_slotKeyInput = new std::map<std::string, int>(*input);
			_keys = new std::vector<std::string>(*keys);
		}

		void LoadingSurvivalState::loadAdvertisement()
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