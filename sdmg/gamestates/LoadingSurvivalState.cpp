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
#include "sdl\include\SDL.h"
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
#include "engine\ai\EasyAIMachine.h"
#include <array>
#include <random>

#include "engine\util\FileManager.h"

namespace sdmg {
	namespace gamestates {

		void LoadingSurvivalState::init(GameBase &game)
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
			if (_enemyNames) {
				for (auto it : *_enemyNames) {
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

		void LoadingSurvivalState::cleanup(GameBase &game)
		{
			delete _levelName;
			delete _playerName;

			if (_enemyNames) {
				for (auto it : *_enemyNames) {
					delete it;
				}
				_enemyNames->clear();
			}
			delete _enemyNames;
			_enemyNames = nullptr;

			game.getEngine()->getDrawEngine()->unload("loading");
			game.getEngine()->getDrawEngine()->unloadText("progress");
		}

		void LoadingSurvivalState::pause(GameBase &game)
		{
		}

		void LoadingSurvivalState::resume(GameBase &game)
		{
		}

		void LoadingSurvivalState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;
			if (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					// game.stop();
				}

				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						// game.stop();
						break;
					}
				}
			}
		}

		void LoadingSurvivalState::update(GameBase &game, GameTime &gameTime)
		{
			if (_isLoaded)
			{
				PlayState &state = (_isTutorial ? TutorialState::getInstance() : PlayState::getInstance());
				state.setHUDs(_huds);
				changeState(game, state);
			}
			if (_isError) {
				// Clean uppen
				changeState(game, MainMenuState::getInstance());
			}
		}

		void LoadingSurvivalState::draw(GameBase &game, GameTime &gameTime)
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

		void LoadingSurvivalState::load() {
			int maxLoadingValue = _totalWidth - (_marginInner * 2) - (_marginValue * 2);
			_loadingStep = maxLoadingValue / 3;

			loadAdvertisement();
			loadLevel();
			loadKeybindings();

			_loadingValue = maxLoadingValue;

			_isLoaded = true;
			clearEventQueue();
		}

		void LoadingSurvivalState::clearEventQueue() {
			SDL_Event event;
			while (SDL_PollEvent(&event));
		}

		void LoadingSurvivalState::loadLevel() {

			_game->getStateManager()->draw();

			JSON::JSONDocument *doc = JSON::JSONDocument::fromFile("assets/levels/" + (*_levelName) + "/data");
			JSON::JSONObject &levelObj = JSON::JSONObject(nullptr);
			levelObj = doc->getRootObject();

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
					_game->getWorld()->addPlatform(platform);
					de->load(platform, "assets/levels/" + (*_levelName) + "/" + platformObj.getString("image"));

					_loadingValue += platformStep;
					_game->getStateManager()->draw();
				}
			}
			else {
				_loadingValue += _loadingStep / 3;
			}

			de->load("background", "assets/levels/" + (*_levelName) + "/background");
			_game->getEngine()->getAudioEngine()->load("bgm", "assets/levels/" + (*_levelName) + "/bgm.mp3", AUDIOTYPE::MUSIC);

			loadCharacters(levelObj.getArray("startingPositions"));

			loadBulletBobs(levelObj.getArray("bobs"));

			de->loadDynamicText("fps", { 255, 255, 255 }, "arial", 18);
			de->loadDynamicText("speed", { 255, 255, 255 }, "arial", 18);
			delete doc;
		}


		void LoadingSurvivalState::loadCharacters(JSON::JSONArray &startingPositions) {
			Character *player = nullptr;
			_enemies = new std::vector<model::Character*>((*_enemyNames).size());

			int characterStep = (_loadingStep / 3) / ((*_enemyNames).size() + 1);
			
			_game->getStateManager()->draw();

			int retries = 0;
			do{
				JSON::JSONObject &posObj = startingPositions.getObject(0);
				player = factories::CharacterFactory::create(*_playerName, *_game, posObj.getFloat("x"), posObj.getFloat("y"));
				_game->getWorld()->addPlayer(player);
				if (retries++ > 10){
					_isError = true;
					return;
				}
			} while (player == nullptr);

			_loadingValue += characterStep;

			for (int i = 0; i < (*_enemyNames).size(); i++) {

				_game->getStateManager()->draw();

				retries = 0;
				do{
						JSON::JSONObject &posObj = startingPositions.getObject(1);
						(*_enemies)[i] = factories::CharacterFactory::create(*(*_enemyNames)[i], *_game, posObj.getFloat("x"), posObj.getFloat("y"));
						(*_enemies)[i]->setDirection(MovableGameObject::Direction::LEFT);
						(*_enemies)[i]->setSpawnDirection(MovableGameObject::Direction::LEFT);
						if (i>0)
							(*_enemies)[i]->getBody()->SetActive(false);
						_game->getWorld()->addPlayer((*_enemies)[i]);
					
					if (retries++ > 10){
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

			HUD *hudPanda = new HUD(*player, 10);
			_huds->push_back(hudPanda);

			HUD *hudNivek = new HUD(*(*_enemies)[0], _game->getEngine()->getDrawEngine()->getWindowWidth() - 230 - 10);
			_huds->push_back(hudNivek);

			_loadingValue += characterStep;
		}

		void LoadingSurvivalState::loadBulletBobs(JSON::JSONArray &bobs) {

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

		void LoadingSurvivalState::loadKeybindings() {

			_game->getStateManager()->draw();

			try{
				ConfigManager &manager = ConfigManager::getInstance();
				InputDeviceBinding *binding = new InputDeviceBinding();

				const std::vector<MovableGameObject*> players = _game->getWorld()->getPlayers();

				int controlStep = _loadingStep;

				_game->getEngine()->getInputEngine()->clearBindings();

				Character *character = static_cast<Character*>(players[0]);
				binding->setKeyBinding(manager.getKey(0, "walkRight"), new actions::RightWalkAction(character));
				binding->setKeyBinding(manager.getKey(0, "walkLeft"), new actions::LeftWalkAction(character));
				binding->setKeyBinding(manager.getKey(0, "jump"), new actions::JumpAction(character));
				binding->setKeyBinding(manager.getKey(0, "roll"), new actions::RollAction(character));
				binding->setKeyBinding(manager.getKey(0, "midrange"), new actions::MidRangeAttackAction(character));
				binding->setKeyBinding(manager.getKey(0, "longrange"), new actions::LongRangeAttackAction(character));
				binding->setKeyBinding(manager.getKey(0, "block"), new actions::BlockAction(character));

				for (size_t i = 1; i < players.size(); i++)
				{
					engine::ai::AIMachine *machine = new engine::ai::EasyAIMachine(*static_cast<Character*>(players[i]), *static_cast<Character*>(players[0]));
					static_cast<Character*>(players[i])->setAI(*machine);

					if (i > 1)
						machine->pause();
				}

				_loadingValue += controlStep;
				_game->getStateManager()->draw();

				_game->getEngine()->getInputEngine()->setDeviceBinding("keyboard", binding);
			}
			catch (...)
			{
				std::cout << "Cannot load keyboard bindings.";
			}
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