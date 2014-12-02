/*

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
#include <array>

#include <random>
// only for windows
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

namespace sdmg {
	namespace gamestates {

		void LoadingState::init(GameBase &game)
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

			game.getEngine()->getDrawEngine()->load("loading", "assets\\screens\\loadingscreen");

			/*
			//  loadAdvertisement();
			std::string advertisement = getRandomAdvertisement();
			if (advertisement != "")
			{
			_isAdvertisement = true;
			game.getEngine()->getDrawEngine()->load("advertisement", "assets\\advertisements\\" + advertisement);
			}

			//  game.getEngine()->getDrawEngine()->loadDynamicText("progress", { 0, 0, 0 }, "trebucbd", 36);
			game.getEngine()->getDrawEngine()->loadDynamicText("progress", { 0, 0, 0 }, "arial", 36);

			_progress = new std::string("Loading started");
			_game->getStateManager()->draw();

			load();
			game.getEngine()->getAudioEngine()->unload("main_menu_bgm");

			_game->getStateManager()->update();

			

			printf("\nSimple SDL_CreateThread test:");

			// Simply create a thread
			thread = SDL_CreateThread(loadThread, "LoadThread", (void *)this);
			//  SDL_WaitThread(thread, NULL);
			//load();
			//  SDL_DetachThread(thread);
			
		}

		void LoadingState::cleanup(GameBase &game)
		{
			delete _level;
			game.getEngine()->getDrawEngine()->unload("loading");
			//  game.getEngine()->getDrawEngine()->unload("progress");
			// game.getEngine()->getAudioEngine()->unload("bgm");
		}

		void LoadingState::pause(GameBase &game)
		{
		}

		void LoadingState::resume(GameBase &game)
		{
		}

		void LoadingState::handleEvents(GameBase &game, GameTime &gameTime)
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

		void LoadingState::update(GameBase &game, GameTime &gameTime)
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

		void LoadingState::setIsTutorial(bool tutorial)
		{
			_isTutorial = tutorial;
		}

		void LoadingState::draw(GameBase &game, GameTime &gameTime)
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
			

			game.getEngine()->getDrawEngine()->drawRectangle(Rectangle(_loadingBarX, _loadingBarY, _totalWidth, _totalHeight), 200, 200, 200);
			game.getEngine()->getDrawEngine()->drawRectangle(Rectangle(_loadingBarX + _marginInner, _loadingBarY + _marginInner, _totalWidth - (_marginInner * 2), _totalHeight - (_marginInner * 2)), 255, 255, 255);
			game.getEngine()->getDrawEngine()->drawRectangle(Rectangle(_loadingBarX + _marginInner + _marginValue, _loadingBarY + _marginInner + _marginValue, _loadingValue, _totalHeight - (_marginInner * 2) - (_marginValue * 2)), 50, 50, 50);

			game.getEngine()->getDrawEngine()->render();
		}

		void LoadingState::setLevel(std::string *level)
		{
			_level = level;
		}

		int LoadingState::loadThread(void *ptr)
		{
			((LoadingState*)ptr)->load();
			return 0;
		}

		void LoadingState::load() {

			int maxLoadingValue = _totalWidth - (_marginInner * 2) - (_marginValue * 2);
			_loadingStep = _isTutorial ? maxLoadingValue / 4 : maxLoadingValue / 3;

			loadAdvertisement();
			loadLevel();
			loadKeybindings();

			if (_isTutorial)
				loadTutorial();

			_loadingValue = maxLoadingValue;

			_isLoaded = true;
		}

		void LoadingState::loadLevel() {

			_progress = new std::string("Loading awesome level!");
			_game->getStateManager()->draw();

			JSON::JSONDocument *doc = JSON::JSONDocument::fromFile("assets/levels/" + (*_level) + "/data");
			JSON::JSONObject &levelObj = doc->getRootObject();

			PhysicsEngine *pe = _game->getEngine()->getPhysicsEngine();
			DrawEngine *de = _game->getEngine()->getDrawEngine();

			pe->setWorldGravity(levelObj.getObject("gravity").getFloat("left"), levelObj.getObject("gravity").getFloat("down"));

			JSON::JSONArray &platformArr = levelObj.getArray("platforms");

			int platformStep = (_loadingStep / 3) / platformArr.size();

			for (int i = 0; i < platformArr.size(); i++) {
				JSON::JSONObject &platformObj = platformArr.getObject(i);

				Platform *platform = new model::Platform(false);
				platform->setSize(platformObj.getObject("size").getFloat("width"), platformObj.getObject("size").getFloat("height"));
				platform->setLocation(platformObj.getObject("location").getFloat("x"), platformObj.getObject("location").getFloat("y"));
				pe->addBody(platform, platformObj.getObject("bodyPadding").getFloat("x"), platformObj.getObject("bodyPadding").getFloat("y"));
				_game->getWorld()->addPlatform(platform);
				de->load(platform, "assets/levels/" + (*_level) + "/" + platformObj.getString("image"));

				_loadingValue += platformStep;
				_game->getStateManager()->draw();
			}
			de->load("background", "assets/levels/" + (*_level) + "/background");
			//  de->load("background", "assets/levels/" + level + "/data");
			//  de->loadText("escape_text", "PRESS 'ESC' TO RETURN TO THE MAINMENU", { 255, 255, 255 }, "arial", 18);
			//  _game->getEngine()->getAudioEngine()->load("level1_bgm", R"(assets/sounds/bgm/level1_bgm.mp3)", AUDIOTYPE::MUSIC);
			_game->getEngine()->getAudioEngine()->load("bgm", "assets/levels/" + (*_level) + "/bgm.mp3", AUDIOTYPE::MUSIC);

			loadCharacters(levelObj.getArray("startingPositions"));

			loadBulletBobs(levelObj.getArray("bobs"));

			// Load fps text
			de->loadDynamicText("fps", { 255, 255, 255 }, "arial", 18);

			delete doc;
		}

		void LoadingState::loadCharacters(JSON::JSONArray &startingPositions) {
			std::string loadCharacters[] = { "fiat", "nivek" };
			std::vector<Character*> characters(sizeof(loadCharacters));

			//int characterStep = (_loadingStep / 3) / (loadCharacters->size() + 1);

			int characterStep = 0;
			if (characterStep <= 0)
				characterStep = (_loadingStep / 3);
			else
				characterStep = (_loadingStep / 3) / (loadCharacters->size() + 1);

			for (int i = 0; i < 2; i++) {

				_progress = new std::string("Loading " + loadCharacters[i]);
				_game->getStateManager()->draw();

				int retries = 0;
				do{
					JSON::JSONObject &posObj = startingPositions.getObject(i);
					characters[i] = factories::CharacterFactory::create(loadCharacters[i], *_game, posObj.getFloat("x"), posObj.getFloat("y"));
					_game->getWorld()->addPlayer(characters[i]);
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

			_progress = new std::string("Loading fancy hudjes");
			_game->getStateManager()->draw();

			// Create a HUD for each player
			_huds = new std::vector<helperclasses::HUD*>();

			HUD *hudPanda = new HUD(*characters[0], 10);
			_huds->push_back(hudPanda);

			HUD *hudNivek = new HUD(*characters[1], _game->getEngine()->getDrawEngine()->getWindowWidth() - 230 - 10);
			_huds->push_back(hudNivek);

			_loadingValue += characterStep;
		}

		void LoadingState::loadBulletBobs(JSON::JSONArray &bobs) {

			_progress = new std::string("Loading Bullet Bobs");
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
				platform->setDirection(static_cast< MovableGameObject::Direction>((int)bobObj.getFloat("direction")));
				platform->setSpeed(bobObj.getObject("speed").getFloat("horizontal"), bobObj.getObject("speed").getFloat("vertical"));
				platform->setDamageOnImpact(100);

				_game->getWorld()->addPlatform(platform);
				_game->getEngine()->getPhysicsEngine()->addKinematicBody(platform);
				_game->getEngine()->getDrawEngine()->loadMap(platform, MovableGameObject::State::IDLE, R"(assets\levels\level2\bullet.sprite)", 1097, 494, 0.1);

				_loadingValue += bobStep;
				_game->getStateManager()->draw();
			}
		}

		void LoadingState::loadKeybindings() {

			_progress = new std::string("Loading controls jonguh!");
			_game->getStateManager()->draw();

			try{
				ConfigManager &manager = ConfigManager::getInstance();
				InputDeviceBinding *binding = new InputDeviceBinding();

				const std::vector<MovableGameObject*> players = _game->getWorld()->getPlayers();

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
				std::cout << "Cannot load keyboard bindings.";
			}
		}

		void LoadingState::loadTutorial() {
			_progress = new std::string("Loading Tutorial");
			_game->getStateManager()->draw();

			DrawEngine *de = _game->getEngine()->getDrawEngine();

			ConfigManager &manager = ConfigManager::getInstance();
			std::string fiatLeft = SDL_GetKeyName(manager.getKey(0, "walkLeft"));
			std::string fiatRight = SDL_GetKeyName(manager.getKey(0, "walkRight"));
			std::string fiatJump = SDL_GetKeyName(manager.getKey(0, "jump"));
			std::string fiatMidRange = SDL_GetKeyName(manager.getKey(0, "midrange"));
			std::string fiatRoll = SDL_GetKeyName(manager.getKey(0, "roll"));

			std::string nivekLeft = SDL_GetKeyName(manager.getKey(1, "walkLeft"));
			std::string nivekRight = SDL_GetKeyName(manager.getKey(1, "walkRight"));
			std::string nivekJump = SDL_GetKeyName(manager.getKey(1, "jump"));
			std::string nivekMidRange = SDL_GetKeyName(manager.getKey(1, "midrange"));
			std::string nivekRoll = SDL_GetKeyName(manager.getKey(1, "roll"));

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
			de->loadText("tutNivek6", "To dodge an enemy attack, execute a roll, press '" + nivekRoll + "'", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutFiat1", "Now we shall explain Fiat's movements [ENTER]", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutFiat2", "Press '" + fiatLeft + "' to move left", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutFiat3", "Press '" + fiatRight + "' to move right", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutFiat4", "Press '" + fiatJump + "' to jump", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutFiat5", "We will now learn attacking movements [ENTER]", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutFiat6", "To perform a mid range attack, press the '" + fiatMidRange + "' key", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutFiat7", "To dodge an enemy attack, execute a roll, press '" + fiatRoll + "'", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutEnd", "You have successfully passed the tutorial, you are ready to play the game, press 'Esc' to quit!", { 255, 255, 255 }, "arial", 30);
		}

		void LoadingState::loadAdvertisement()
		{
			_progress = new std::string("Loading Advertisement");
			_game->getStateManager()->draw();

			std::string advertisement = getRandomAdvertisement();
			if (advertisement != "")
			{
				_isAdvertisement = true;
				_game->getEngine()->getDrawEngine()->load("advertisement", "assets\\advertisements\\" + advertisement);

				const std::array<float, 2> size = _game->getEngine()->getDrawEngine()->getImageSize("advertisement");
				_advertisementX = _game->getEngine()->getDrawEngine()->getWindowWidth() - size[0] - 10;
				_advertisementY = _game->getEngine()->getDrawEngine()->getWindowHeight() - size[1] - 10;
			}

			_loadingValue += _loadingStep;
		}

		std::string LoadingState::getRandomAdvertisement() {

			std::vector<std::string> names;
			char search_path[200];
			sprintf_s(search_path, "%s*.*", "assets\\advertisements\\");
			WIN32_FIND_DATA fd;
			HANDLE hFind = ::FindFirstFile(search_path, &fd);
			if (hFind != INVALID_HANDLE_VALUE)
			{
				do {
					if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						//_tprintf(TEXT("  %s   <DIR>\n"), fd.cFileName);
					}
					else
					{
						names.push_back(fd.cFileName);
					}
				} while (::FindNextFile(hFind, &fd));
				::FindClose(hFind);
			}

			if (names.size() > 0)
			{
				std::random_device dev;
				std::default_random_engine dre(dev());
				std::uniform_int_distribution<int> randomAdvertisement(0, names.size() - 1);

				return names[randomAdvertisement(dre)];
			}

			return "";
		}
	}
}
*/