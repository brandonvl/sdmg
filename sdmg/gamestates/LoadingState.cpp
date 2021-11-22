//
//
//  @ Project : SDMG
//  @ File Name : LoadingState.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "LoadingState.h"
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
#include <array>

#include "engine\ai\EasyAIMachine.h"

#include "engine\util\FileManager.h"

#include <random>
// only for windows
// Advertisement ophalen
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

namespace sdmg
{
	namespace gamestates
	{

		void LoadingState::init(GameBase& game)
		{
			_game = &game;
			_game->getWorld()->clearWorld();
			//  _game->getStateManager()->cleanupOthers();

			// Default level
			// _level = new std::string("level1");

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
			_progress = new std::string();

			game.getEngine()->getDrawEngine()->load("loading", "assets\\screens\\loadingscreen");

			/*
			//  loadAdvertisement();
			std::string advertisement = getRandomAdvertisement();
			if (advertisement != "")
			{
			_isAdvertisement = true;
			game.getEngine()->getDrawEngine()->load("advertisement", "assets\\advertisements\\" + advertisement);
			}*/

			//  game.getEngine()->getDrawEngine()->loadDynamicText("progress", { 0, 0, 0 }, "trebucbd", 36);
			game.getEngine()->getDrawEngine()->loadDynamicText("progress", { 0, 0, 0 }, "arial", 36);

			*_progress = "Loading started";
			_game->getStateManager()->draw();

			game.getEngine()->getAudioEngine()->load("hurt_punch", "assets\\sounds\\effects\\hurt_punch.ogg", AUDIOTYPE::SOUND_EFFECT);
			game.getEngine()->getAudioEngine()->load("hurt_shoot", "assets\\sounds\\effects\\hurt_shoot.ogg", AUDIOTYPE::SOUND_EFFECT);
			load();
			game.getEngine()->getAudioEngine()->unload("main_menu_bgm");

			_game->getEngine()->getPhysicsEngine()->resume();

			_game->getStateManager()->update();

			/*

		printf("\nSimple SDL_CreateThread test:");

		// Simply create a thread
		thread = SDL_CreateThread(loadThread, "LoadThread", (void *)this);
		//  SDL_WaitThread(thread, NULL);
		//load();
		//  SDL_DetachThread(thread);
		*/
		}

		void LoadingState::cleanup(GameBase& game)
		{
			delete _level;
			_level = nullptr;
			delete _progress;
			_progress = nullptr;
			delete _characters;
			_characters = nullptr;

			delete _slotKeyInput;
			_slotKeyInput = nullptr;
			delete _keys;
			_keys = nullptr;

			game.getEngine()->getDrawEngine()->unload("loading");
			game.getEngine()->getDrawEngine()->unloadText("progress");
		}

		void LoadingState::pause(GameBase& game)
		{
		}

		void LoadingState::resume(GameBase& game)
		{
		}

		void LoadingState::handleEvents(GameBase& game, GameTime& gameTime)
		{
			SDL_Event event;
			while (SDL_PollEvent(&event));
		}

		void LoadingState::update(GameBase& game, GameTime& gameTime)
		{
			if (_isLoaded)
			{
				PlayState& state = (_isTutorial ? TutorialState::getInstance() : PlayState::getInstance());
				if (game.getGameMode() != GameBase::GameMode::Edit && !_isTutorial)
					state.setSlotKeyBinding(_slotKeyInput, _keys);

				state.setLevel(_level);
				state.setHUDs(_huds);
				changeState(game, state);
			}
			if (_isError)
			{
				// Clean uppen
				changeState(game, MainMenuState::getInstance());
			}
		}

		void LoadingState::setIsTutorial(bool tutorial)
		{
			_isTutorial = tutorial;
		}

		void LoadingState::draw(GameBase& game, GameTime& gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("loading");

			if (_isAdvertisement)
				game.getEngine()->getDrawEngine()->draw("advertisement", _advertisementX, _advertisementY);

			//game.getEngine()->getDrawEngine()->drawDynamicText("progress", (*_progress), 100, 100);

			/*
			int marginInner = 3, marginValue = 1;
			int totalWidth = 300, totalHeight = 23;
			int x = game.getEngine()->getDrawEngine()->getWindowWidth() / 2 - totalWidth / 2, y = 565;
			*/

			game.getEngine()->getDrawEngine()->drawRectangle(Rectangle(_loadingBarX, _loadingBarY, _totalWidth, _totalHeight), 200, 200, 200);
			game.getEngine()->getDrawEngine()->drawRectangle(Rectangle(_loadingBarX + _marginInner, _loadingBarY + _marginInner, _totalWidth - (_marginInner * 2), _totalHeight - (_marginInner * 2)), 255, 255, 255);
			game.getEngine()->getDrawEngine()->drawRectangle(Rectangle(_loadingBarX + _marginInner + _marginValue, _loadingBarY + _marginInner + _marginValue, _loadingValue, _totalHeight - (_marginInner * 2) - (_marginValue * 2)), 50, 50, 50);

			game.getEngine()->getDrawEngine()->render();
		}

		void LoadingState::setLevel(std::string* level)
		{
			_level = level;
		}

		void LoadingState::setSlotKeyBinding(std::map<std::string, int>* input, std::vector<std::string>* keys)
		{
			if (_slotKeyInput != nullptr)
				delete _slotKeyInput;

			if (_keys != nullptr)
				delete _keys;

			_slotKeyInput = new std::map<std::string, int>(*input);
			_keys = new std::vector<std::string>(*keys);
		}

		int LoadingState::loadThread(void* ptr)
		{
			((LoadingState*)ptr)->load();
			return 0;
		}

		void LoadingState::load()
		{
			int maxLoadingValue = _totalWidth - (_marginInner * 2) - (_marginValue * 2);
			_loadingStep = _isTutorial ? maxLoadingValue / 4 : maxLoadingValue / 3;

			loadAdvertisement();
			loadLevel();
			loadKeybindings();

			if (_isTutorial)
				loadTutorial();

			_loadingValue = maxLoadingValue;

			_isLoaded = true;
			clearEventQueue();
		}

		void LoadingState::clearEventQueue()
		{
			SDL_Event event;
			while (SDL_PollEvent(&event));
		}

		void LoadingState::loadLevel()
		{

			*_progress = "Loading awesome level!";
			_game->getStateManager()->draw();

			std::string folder = "assets/levels/" + (*_level);
			auto json = engine::util::FileManager::getInstance().loadFileContentsAsJson(folder + "/data");
			auto jsonGravityObject = json["gravity"];
			PhysicsEngine* pe = _game->getEngine()->getPhysicsEngine();
			DrawEngine* de = _game->getEngine()->getDrawEngine();

			pe->setWorldGravity(jsonGravityObject["left"].get<float>(), jsonGravityObject["down"].get<float>());

			auto& platformArr = json["platforms"];

			if (platformArr.size() > 0)
			{
				int platformStep = (_loadingStep / 3) / platformArr.size();

				for (auto& platformObj : platformArr)
				{

					Platform* platform = new model::Platform();
					platform->setSize(platformObj["size"]["width"].get<float>(), platformObj["size"]["height"].get<float>());
					platform->setLocation(platformObj["location"]["x"].get<float>(), platformObj["location"]["y"].get<float>());
					pe->addBody(platform, platformObj["bodyPadding"]["x"].get<float>(), platformObj["bodyPadding"]["y"].get<float>());

					if (platformObj.contains("canMoveThroughIt"))
						platform->setCanMoveThroughIt(false);
					else
						platform->setCanMoveThroughIt(true);

					_game->getWorld()->addPlatform(platform);
					if (platformObj.contains("image"))
						de->load(platform, "assets/levels/" + (*_level) + "/" + platformObj["image"].get<std::string>());

					_loadingValue += platformStep;
					_game->getStateManager()->draw();
				}
			}
			else
			{
				_loadingValue += _loadingStep / 3;
			}

			de->load("overlay", "assets/levels/" + (*_level) + "/overlay");
			de->load("background", "assets/levels/" + (*_level) + "/background");
			//  de->load("background", "assets/levels/" + level + "/data");
			//  de->loadText("escape_text", "PRESS 'ESC' TO RETURN TO THE MAINMENU", { 255, 255, 255 }, "arial", 18);
			//  _game->getEngine()->getAudioEngine()->load("level1_bgm", R"(assets/sounds/bgm/level1_bgm.mp3)", AUDIOTYPE::MUSIC);
			_game->getEngine()->getAudioEngine()->load("bgm", "assets/levels/" + (*_level) + "/bgm.mp3", AUDIOTYPE::MUSIC);

			loadCharacters(json["startingPositions"]);

			if (json.contains("bobs"))
				loadBulletBobs(json["bobs"]);

			// Load fps text
			de->loadDynamicText("fps", { 255, 255, 255 }, "arial", 18);
			de->loadDynamicText("speed", { 255, 255, 255 }, "arial", 18);
		}

		void LoadingState::loadCharacters(nlohmann::json& startingPositions)
		{

			if (_characters == nullptr)
			{
				_characters = new std::vector<std::string>();
				_characters->push_back("fiat");
				_characters->push_back("nivek");
			}

			auto &loadCharacters = *_characters;
			std::vector<Character*> characters(sizeof(loadCharacters));

			int characterStep = (_loadingStep / 3) / (*_characters).size();

			for (int i = 0; i < (*_characters).size(); i++)
			{

				*_progress = "Loading " + loadCharacters[i];
				_game->getStateManager()->draw();

				int retries = 0;
				do
				{
					auto& posObj = startingPositions[i];
					characters[i] = factories::CharacterFactory::create(loadCharacters[i], *_game, posObj["x"].get<float>(), posObj["y"].get<float>());
					_game->getWorld()->addPlayer(characters[i]);
					if (retries++ > 10)
					{
						_isError = true;
						return;
					}
				} while (characters[i] == nullptr);

				_loadingValue += characterStep;
				_game->getStateManager()->draw();
			}

			/*
			engine::ai::AIMachine *machine1 = new engine::ai::EasyAIMachine(*characters[0], *characters[1]);
			characters[0]->setAI(*machine1);

			engine::ai::AIMachine *machine2 = new engine::ai::EasyAIMachine(*characters[1], *characters[0]);
			characters[1]->setAI(*machine2);

			if ((*_characters).size() >= 3)
			{
				engine::ai::AIMachine *machine3 = new engine::ai::EasyAIMachine(*characters[2], *characters[1]);
				characters[2]->setAI(*machine3);
			}

			if ((*_characters).size() >= 4)
			{
				engine::ai::AIMachine *machine4 = new engine::ai::EasyAIMachine(*characters[3], *characters[2]);
				characters[3]->setAI(*machine4);
			}
			*/

			* _progress = "Loading fancy hudjes";
			_game->getStateManager()->draw();

			// Create a HUD for each player
			_huds = new std::vector<helperclasses::HUD*>();

			HUD* hud1 = new HUD(*characters[0], 10);
			_huds->push_back(hud1);

			HUD* hud2 = new HUD(*characters[1], _game->getEngine()->getDrawEngine()->getWindowWidth() - 230 - 10);
			_huds->push_back(hud2);

			if ((*_characters).size() >= 3)
			{
				HUD* hud3 = new HUD(*characters[2], 20 + 230);
				_huds->push_back(hud3);
			}
			if ((*_characters).size() >= 4)
			{
				HUD* hud4 = new HUD(*characters[3], _game->getEngine()->getDrawEngine()->getWindowWidth() - (230 * 2) - 20);
				_huds->push_back(hud4);
			}

			_loadingValue += characterStep;
		}

		void LoadingState::loadBulletBobs(nlohmann::json& bobs)
		{

			*_progress = "Loading Bullet Bobs";
			_game->getStateManager()->draw();

			int bobStep = 0;
			if (bobStep <= 0)
				_loadingValue += (_loadingStep / 3);
			else
				bobStep = (_loadingStep / 3) / bobs.size();

			for (int i = 0; i < bobs.size(); i++)
			{
				auto& bobObj = bobs[i];

				MovablePlatform* platform = new MovablePlatform();
				platform->setSize(bobObj["size"]["width"].get<float>(), bobObj["size"]["height"].get<float>());
				platform->setLocation(bobObj["location"]["x"].get<float>(), bobObj["location"]["y"].get<float>());
				platform->setStartLocation(b2Vec2(bobObj["location"]["x"].get<float>(), bobObj["location"]["y"].get<float>()));
				platform->setEndLocation(b2Vec2(bobObj["endLocation"]["x"].get<float>(), bobObj["endLocation"]["y"].get<float>()));
				platform->setDirection(static_cast<MovableGameObject::Direction>((int)bobObj["direction"].get<float>()));
				platform->setSpeed(bobObj["speed"]["horizontal"].get<float>(), bobObj["speed"]["vertical"].get<float>());
				platform->setDamageOnImpact(100);

				_game->getWorld()->addPlatform(platform);
				_game->getEngine()->getPhysicsEngine()->addKinematicBody(platform);
				_game->getEngine()->getDrawEngine()->loadMap(platform, MovableGameObject::State::IDLE, R"(assets\levels\level2\bullet.sprite)", 1097, 494, 0.1);

				_loadingValue += bobStep;
				_game->getStateManager()->draw();
			}
		}

		void LoadingState::loadKeybindings()
		{

			*_progress = "Loading controls";
			_game->getStateManager()->draw();

			try
			{
				ConfigManager& manager = ConfigManager::getInstance();


				const std::vector<MovableGameObject*> players = _game->getWorld()->getPlayers();

				_game->getEngine()->getInputEngine()->clearBindings();


				int controlStep = (_loadingStep) / players.size();

				if (_isTutorial)
				{
					std::string keyBinding = "keyboardRIGHT";
					InputDeviceBinding* binding = _game->getEngine()->getInputEngine()->createBinding(keyBinding);
					int deviceIndexInFile = manager.getDeviceIndex(keyBinding);
					Character* character = static_cast<Character*>(players[1]);

					binding->setKeyBinding(manager.getKey(deviceIndexInFile, "walkRight"), new actions::RightWalkAction(character));
					binding->setKeyBinding(manager.getKey(deviceIndexInFile, "walkLeft"), new actions::LeftWalkAction(character));
					binding->setKeyBinding(manager.getKey(deviceIndexInFile, "jump"), new actions::JumpAction(character));
					binding->setKeyBinding(manager.getKey(deviceIndexInFile, "roll"), new actions::RollAction(character));
					binding->setKeyBinding(manager.getKey(deviceIndexInFile, "midrange"), new actions::MidRangeAttackAction(character));
					binding->setKeyBinding(manager.getKey(deviceIndexInFile, "longrange"), new actions::LongRangeAttackAction(character));
					binding->setKeyBinding(manager.getKey(deviceIndexInFile, "block"), new actions::BlockAction(character));
					_game->getEngine()->getInputEngine()->setDeviceBinding(keyBinding, binding);
				}
				else if (_game->getGameMode() == GameBase::GameMode::Edit)
				{
					std::string keyBinding = "keyboardLEFT";
					InputDeviceBinding* binding = _game->getEngine()->getInputEngine()->createBinding(keyBinding);
					int deviceIndexInFile = manager.getDeviceIndex(keyBinding);
					Character* character = static_cast<Character*>(players[0]);

					binding->setKeyBinding(manager.getKey(deviceIndexInFile, "walkRight"), new actions::RightWalkAction(character));
					binding->setKeyBinding(manager.getKey(deviceIndexInFile, "walkLeft"), new actions::LeftWalkAction(character));
					binding->setKeyBinding(manager.getKey(deviceIndexInFile, "jump"), new actions::JumpAction(character));
					binding->setKeyBinding(manager.getKey(deviceIndexInFile, "roll"), new actions::RollAction(character));
					binding->setKeyBinding(manager.getKey(deviceIndexInFile, "midrange"), new actions::MidRangeAttackAction(character));
					binding->setKeyBinding(manager.getKey(deviceIndexInFile, "longrange"), new actions::LongRangeAttackAction(character));
					binding->setKeyBinding(manager.getKey(deviceIndexInFile, "block"), new actions::BlockAction(character));
					_game->getEngine()->getInputEngine()->setDeviceBinding(keyBinding, binding);

					std::string keyBinding2 = "keyboardRIGHT";
					InputDeviceBinding* binding2 = _game->getEngine()->getInputEngine()->createBinding(keyBinding2);
					int deviceIndexInFile2 = manager.getDeviceIndex(keyBinding2);
					Character* character2 = static_cast<Character*>(players[1]);

					binding2->setKeyBinding(manager.getKey(deviceIndexInFile2, "walkRight"), new actions::RightWalkAction(character2));
					binding2->setKeyBinding(manager.getKey(deviceIndexInFile2, "walkLeft"), new actions::LeftWalkAction(character2));
					binding2->setKeyBinding(manager.getKey(deviceIndexInFile2, "jump"), new actions::JumpAction(character2));
					binding2->setKeyBinding(manager.getKey(deviceIndexInFile2, "roll"), new actions::RollAction(character2));
					binding2->setKeyBinding(manager.getKey(deviceIndexInFile2, "midrange"), new actions::MidRangeAttackAction(character2));
					binding2->setKeyBinding(manager.getKey(deviceIndexInFile2, "longrange"), new actions::LongRangeAttackAction(character2));
					binding2->setKeyBinding(manager.getKey(deviceIndexInFile2, "block"), new actions::BlockAction(character2));
					_game->getEngine()->getInputEngine()->setDeviceBinding(keyBinding2, binding2);
				}
				else
				{
					for (int i = 0; i < players.size(); i++)
					{
						std::string keyBinding = getSlotKeyInput(i);
						InputDeviceBinding* binding = _game->getEngine()->getInputEngine()->createBinding(keyBinding);
						int deviceIndexInFile = manager.getDeviceIndex(keyBinding);
						Character* character = static_cast<Character*>(players[i]);

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
					}
				}

				//_game->getEngine()->getInputEngine()->setDeviceBinding("keyboard", binding);

			}
			catch (...)
			{
				std::cout << "Cannot load bindings.";
			}
		}

		void LoadingState::loadTutorial()
		{
			*_progress = "Loading Tutorial";
			_game->getStateManager()->draw();

			DrawEngine* de = _game->getEngine()->getDrawEngine();

			ConfigManager& manager = ConfigManager::getInstance();
			//std::string fiatLeft = SDL_GetKeyName(manager.getKey(0, "walkLeft"));
			//std::string fiatRight = SDL_GetKeyName(manager.getKey(0, "walkRight"));
			//std::string fiatJump = SDL_GetKeyName(manager.getKey(0, "jump"));
			//std::string fiatMidRange = SDL_GetKeyName(manager.getKey(0, "midrange"));
			//std::string fiatRoll = SDL_GetKeyName(manager.getKey(0, "roll"));

			std::string nivekLeft = SDL_GetKeyName(manager.getKey(2, "walkLeft"));
			std::string nivekRight = SDL_GetKeyName(manager.getKey(2, "walkRight"));
			std::string nivekJump = SDL_GetKeyName(manager.getKey(2, "jump"));
			std::string nivekMidRange = SDL_GetKeyName(manager.getKey(2, "midrange"));
			std::string nivekRoll = SDL_GetKeyName(manager.getKey(2, "roll"));
			std::string nivekLongRange = SDL_GetKeyName(manager.getKey(2, "longrange"));
			std::string nivekBlock = SDL_GetKeyName(manager.getKey(2, "block"));

			//de->loadText("tutIntro", "Welcome to the S.D.M.G. tutorial!", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutIntro", "Welcome to the tutorial! [ENTER]", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutIntro2", "The heart icons in the top of your screen show the amount of lives you have left [ENTER]", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutIntro3", "If you get defeated by an opponent or fall from the platform you will lose one life [ENTER]", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutIntro4", "The bar below the heart icons shows the energy you have left [ENTER]", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutNivek1", "We shall begin with Nivek, Press '" + nivekLeft + "' to move left", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutNivek2", "Press '" + nivekRight + "' to move right", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutNivek3", "Press '" + nivekJump + "' to jump", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutNivek4", "We will now learn attacking movements [ENTER]", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutNivek5", "To perform a mid range attack, press the '" + nivekMidRange + "' key", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutNivek6", "To perform a long range attack, press the '" + nivekLongRange + "' key", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutNivek7", "To block an enemy attack, press '" + nivekBlock + "'", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutNivek8", "To dodge an enemy attack, execute a roll, press '" + nivekRoll + "'", { 255, 255, 255 }, "arial", 30);
			//de->loadText("tutFiat1", "Now we shall explain Fiat's movements [ENTER]", { 255, 255, 255 }, "arial", 30);
			//de->loadText("tutFiat2", "Press '" + fiatLeft + "' to move left", { 255, 255, 255 }, "arial", 30);
			//de->loadText("tutFiat3", "Press '" + fiatRight + "' to move right", { 255, 255, 255 }, "arial", 30);
			//de->loadText("tutFiat4", "Press '" + fiatJump + "' to jump", { 255, 255, 255 }, "arial", 30);
			//de->loadText("tutFiat5", "We will now learn attacking movements [ENTER]", { 255, 255, 255 }, "arial", 30);
			//de->loadText("tutFiat6", "To perform a mid range attack, press the '" + fiatMidRange + "' key", { 255, 255, 255 }, "arial", 30);
			//de->loadText("tutFiat7", "To dodge an enemy attack, execute a roll, press '" + fiatRoll + "'", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutEnd", "You have successfully passed the tutorial, you are ready to play the game, press [ESC] to quit", { 255, 255, 255 }, "arial", 30);
		}

		void LoadingState::loadAdvertisement()
		{
			*_progress = "Loading Advertisement";

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

		std::string LoadingState::getSlotKeyInput(int slot)
		{
			std::string key = "slot_key_" + std::to_string(slot);
			auto it = _slotKeyInput->find(key);
			if (it != _slotKeyInput->end())
				return _keys->at(it->second);
			else
				return "";
		}
	}
}