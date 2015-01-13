//
//
//  @ Project : SDMG
//  @ File Name : LoadingPlayBackState.cpp
//  @ Date : 18-12-2014
//  @ Author : 42IN13SAd
//
//


#include "LoadingPlayBackState.h"
#include "MainMenuState.h"
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
#include "engine\audio\AudioEngine.h"
#include "helperclasses\HUD.h"
#include "engine\util\FileParser.h"
#include "lib\JSONParser.h"
#include "helperclasses\ConfigManager.h"
#include <array>

#include "engine\util\FileManager.h"

#include <random>

namespace sdmg {
	namespace gamestates {

		void LoadingPlayBackState::init(GameBase &game)
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

			_isLoaded = false;
			_isError = false;
			_isAdvertisement = false;
			_recordQueue = new std::queue<PlayBackState::RecordStep*>();

			game.getEngine()->getDrawEngine()->load("loading", "assets\\screens\\loadingscreen");

			_game->getStateManager()->draw();

			load();
			game.getEngine()->getAudioEngine()->unload("main_menu_bgm");

			_game->getEngine()->getPhysicsEngine()->resume();

			_game->getStateManager()->update();
		}

		void LoadingPlayBackState::cleanup(GameBase &game)
		{
			delete _level;
			_level = nullptr;
			delete _fileName;
			_fileName = nullptr;

			for (auto it : *_characters)
				delete it;
			_characters->clear();
			delete _characters;
			_characters = nullptr;

			_recordQueue = nullptr;
			
			if (_recordMap->size() > 0) {
				std::unordered_map<std::string, Action*>::iterator itr = _recordMap->begin();
				while (itr != _recordMap->end()) {
					delete itr->second;
					_recordMap->erase(itr++);
				}
			}
			delete _recordMap;
			_recordMap = nullptr;

			delete _characterObjects;
			_characterObjects = nullptr;

			game.getEngine()->getDrawEngine()->unload("loading");
		}

		void LoadingPlayBackState::pause(GameBase &game)
		{
		}

		void LoadingPlayBackState::resume(GameBase &game)
		{
		}

		void LoadingPlayBackState::handleEvents(GameBase &game, GameTime &gameTime)
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

		void LoadingPlayBackState::update(GameBase &game, GameTime &gameTime)
		{
			if (_isLoaded)
			{
				PlayBackState::getInstance().setHUDs(_huds);
				changeState(game, PlayBackState::getInstance());
			}
			if (_isError) {
				// Clean uppen
				changeState(game, MainMenuState::getInstance());
			}
		}
		
		void LoadingPlayBackState::draw(GameBase &game, GameTime &gameTime)
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

		void LoadingPlayBackState::setPlaybackFileName(std::string fileName)
		{
			_fileName = new std::string(fileName);
		}

		void LoadingPlayBackState::load() {
			int maxLoadingValue = _totalWidth - (_marginInner * 2) - (_marginValue * 2);
			_loadingStep = maxLoadingValue / 3;

			loadAdvertisement();
			loadKeybindings();
			loadPlaybackSteps();
			
			_loadingValue = maxLoadingValue;

			_isLoaded = true;
			clearEventQueue();
		}

		void LoadingPlayBackState::loadPlaybackSteps()
		{
			JSON::JSONDocument *doc = JSON::JSONDocument::fromFile("assets/playbacks/" + *_fileName);
			JSON::JSONObject &obj = doc->getRootObject();

			_level = new std::string(obj.getString("level"));

			int numberOfCharacters = obj.getArray("characters").size();
			_characters = new std::vector<std::string*>(numberOfCharacters);
			for (int i = 0; i < numberOfCharacters; i++)
			{
				(*_characters)[obj.getArray("characters").getObject(i).getInt("index")] = new std::string(obj.getArray("characters").getObject(i).getString("key"));
			}

			loadLevel();


			// load steps
			_recordQueue = new std::queue<PlayBackState::RecordStep*>();

			int numberOfSteps = obj.getArray("steps").size();
			for (int i = 0; i < numberOfSteps; i++)
			{
				JSON::JSONObject &stepObj = obj.getArray("steps").getObject(i);

				std::string name = stepObj.getString("action") + std::to_string(stepObj.getInt("character"));

				// create fake event
				SDL_Event event;
				event.type = stepObj.getBoolean("keyDown") ? SDL_KEYDOWN : SDL_KEYUP;

				// create and run action
				auto action = (*_recordMap)[name]->create(event);

				auto player = (*_characterObjects)[stepObj.getInt("character")];
				auto step = new PlayBackState::RecordStep(action, stepObj.getFloat("timestamp"), player);

				auto &players = stepObj.getArray("characters");

				for (int i = 0; i < players.size(); i++) {
					auto &player = players.getObject(i);
					auto character = (*_characterObjects)[player.getInt("character")];
					step->_playerData.push_back({ character, player.getInt("hp"), player.getInt("lives"), player.getInt("pp"), player.getFloat("x"), player.getFloat("y"), player.getFloat("velocityx"), player.getFloat("velocityy") });
				}

				_recordQueue->push(step);
			}
			PlayBackState::getInstance().setPlaybackSteps(_recordQueue);

			delete doc;
		}

		void LoadingPlayBackState::clearEventQueue() {
			SDL_Event event;
			while (SDL_PollEvent(&event));
		}

		void LoadingPlayBackState::loadLevel() {

			_game->getStateManager()->draw();

			JSON::JSONDocument *doc = JSON::JSONDocument::fromFile("assets/levels/" + (*_level) + "/data");
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
						de->load(platform, "assets/levels/" + (*_level) + "/" + platformObj.getString("image"));

					_loadingValue += platformStep;
					_game->getStateManager()->draw();
				}
			}
			else {
				_loadingValue += _loadingStep / 3;
			}

			de->load("overlay", "assets/levels/" + (*_level) + "/overlay");
			de->load("background", "assets/levels/" + (*_level) + "/background");
			_game->getEngine()->getAudioEngine()->load("bgm", "assets/levels/" + (*_level) + "/bgm.mp3", AUDIOTYPE::MUSIC);

			loadCharacters(levelObj.getArray("startingPositions"));

			if (levelObj.exists("bobs"))
				loadBulletBobs(levelObj.getArray("bobs"));

			// Load fps text
			de->loadDynamicText("fps", { 255, 255, 255 }, "arial", 18);
			de->loadDynamicText("speed", { 255, 255, 255 }, "arial", 18);
			delete doc;
		}

		void LoadingPlayBackState::loadCharacters(JSON::JSONArray &startingPositions) {

			delete _recordMap;
			_recordMap = new std::unordered_map<std::string, Action*>();

			_characterObjects = new std::vector<Character*>(_characters->size());
			
			int characterStep = (_loadingStep / 3) / (_characters->size() + 1);

			for (int i = 0; i < _characters->size(); i++) {

				_game->getStateManager()->draw();

				int retries = 0;
				do{
					JSON::JSONObject &posObj = startingPositions.getObject(i);
					(*_characterObjects)[i] = factories::CharacterFactory::create(*(*_characters)[i], *_game, posObj.getFloat("x"), posObj.getFloat("y"));
					_game->getWorld()->addPlayer((*_characterObjects)[i]);
					if (retries++ > 10){
						_isError = true;
						return;
					}

					_recordMap->insert({ "BlockAction" + std::to_string(i), new actions::BlockAction((*_characterObjects)[i]) });
					_recordMap->insert({ "JumpAction" + std::to_string(i), new actions::JumpAction((*_characterObjects)[i]) });
					_recordMap->insert({ "LeftWalkAction" + std::to_string(i), new actions::LeftWalkAction((*_characterObjects)[i]) });
					_recordMap->insert({ "LongRangeAttackAction" + std::to_string(i), new actions::LongRangeAttackAction((*_characterObjects)[i]) });
					_recordMap->insert({ "MidRangeAttackAction" + std::to_string(i), new actions::MidRangeAttackAction((*_characterObjects)[i]) });
					_recordMap->insert({ "RespawnAction" + std::to_string(i), new actions::RespawnAction((*_characterObjects)[i]) });
					_recordMap->insert({ "RightWalkAction" + std::to_string(i), new actions::RightWalkAction((*_characterObjects)[i]) });
					_recordMap->insert({ "RollAction" + std::to_string(i), new actions::RollAction((*_characterObjects)[i]) });
					
				} while ((*_characterObjects)[i] == nullptr);

				_loadingValue += characterStep;
				_game->getStateManager()->draw();
			}

			(*_characterObjects)[1]->setDirection(MovableGameObject::Direction::LEFT);
			(*_characterObjects)[1]->setSpawnDirection(MovableGameObject::Direction::LEFT);




			_game->getStateManager()->draw();

			// Create a HUD for each player
			_huds = new std::vector<helperclasses::HUD*>();

			HUD *hud1 = new HUD(*(*_characterObjects)[0], 10);
			_huds->push_back(hud1);

			HUD *hud2 = new HUD(*(*_characterObjects)[1], _game->getEngine()->getDrawEngine()->getWindowWidth() - 230 - 10);
			_huds->push_back(hud2);

			if ((*_characterObjects).size() >= 3)
			{
				HUD *hud3 = new HUD(*(*_characterObjects)[2], 20 + 230);
				_huds->push_back(hud3);
			}
			if ((*_characterObjects).size() >= 4)
			{
				HUD *hud4 = new HUD(*(*_characterObjects)[3], _game->getEngine()->getDrawEngine()->getWindowWidth() - (230 * 2) - 20);
				_huds->push_back(hud4);
			}

			_loadingValue += characterStep;
		}

		void LoadingPlayBackState::loadBulletBobs(JSON::JSONArray &bobs) {

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

		void LoadingPlayBackState::loadKeybindings() {

			_loadingValue += _loadingStep;
			_game->getStateManager()->draw();

			/*
			try{
				ConfigManager &manager = ConfigManager::getInstance();


				const std::vector<MovableGameObject*> players = _game->getWorld()->getPlayers();
				InputDeviceBinding *binding = new InputDeviceBinding();
				//  int controlStep = (_loadingStep / 3) / players.size();
				int controlStep = 0;
				if (controlStep <= 0)
					_loadingValue += (_loadingStep / 3);
				else
					controlStep = (_loadingStep / 3) / players.size();

				_game->getEngine()->getInputEngine()->clearBindings();

				for (int i = 0; i < players.size(); i++)
				{

					Character *character = static_cast<Character*>(players[i]);
					binding->setKeyBinding(manager.getKey(i, "walkRight"), new actions::RightWalkAction(character));
					binding->setKeyBinding(manager.getKey(i, "walkLeft"), new actions::LeftWalkAction(character));
					binding->setKeyBinding(manager.getKey(i, "jump"), new actions::JumpAction(character));
					binding->setKeyBinding(manager.getKey(i, "roll"), new actions::RollAction(character));
					binding->setKeyBinding(manager.getKey(i, "midrange"), new actions::MidRangeAttackAction(character));
					binding->setKeyBinding(manager.getKey(i, "longrange"), new actions::LongRangeAttackAction(character));
					binding->setKeyBinding(manager.getKey(i, "block"), new actions::BlockAction(character));


					_loadingValue += controlStep;
					_game->getStateManager()->draw();

				}

				_game->getEngine()->getInputEngine()->setDeviceBinding("keyboard", binding);

			}
			catch (...)
			{
				std::cout << "Cannot load bindings.";
			}
			*/
		}

		void LoadingPlayBackState::loadAdvertisement()
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