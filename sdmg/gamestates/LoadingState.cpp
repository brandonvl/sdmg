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

namespace sdmg {
	namespace gamestates {

		void LoadingState::init(GameBase &game)
		{
			_game = &game;
			_game->getWorld()->clearWorld();

			_isLoaded = false;
			_isError = false;

			game.getEngine()->getDrawEngine()->load("loading", R"(assets\screens\loadingscreen)");

			printf("\nSimple SDL_CreateThread test:");

			// Simply create a thread
			thread = SDL_CreateThread(loadThread, "LoadThread", (void *)this);
			//  SDL_WaitThread(thread, NULL);
			//load();
			//SDL_DetachThread(thread);

		}

		void LoadingState::cleanup(GameBase &game)
		{
			game.getEngine()->getDrawEngine()->unload("loading");
			game.getEngine()->getAudioEngine()->unload("main_menu_bgm");
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
				state.setCharacters(_characters);
				state.setPlatform(_platform);
				state.setBullets(_bullets);
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
			game.getEngine()->getDrawEngine()->render();
		}

		int LoadingState::loadThread(void *ptr)
		{
			((LoadingState*)ptr)->load();
			return 0;
		}

		void LoadingState::load() {
			loadLevel();
			loadKeybindings();

			if (_isTutorial)
				loadTutorial();

			_isLoaded = true;
		}

		void LoadingState::loadLevel() {

			JSON::JSONDocument *doc = JSON::JSONDocument::fromFile("assets/levels/level2/data");
			JSON::JSONObject &levelObj = doc->getRootObject();

			PhysicsEngine *pe = _game->getEngine()->getPhysicsEngine();
			DrawEngine *de = _game->getEngine()->getDrawEngine();

			pe->setWorldGravity(levelObj.getObject("gravity").getFloat("left"), levelObj.getObject("gravity").getFloat("down"));

			JSON::JSONArray &platformArr = levelObj.getArray("platforms");

			for (int i = 0; i < platformArr.size(); i++) {
				JSON::JSONObject &platformObj = platformArr.getObject(i);

				_platform = new model::Platform(false);
				_platform->setSize(platformObj.getObject("size").getFloat("width"), platformObj.getObject("size").getFloat("height"));
				_platform->setLocation(platformObj.getObject("location").getFloat("x"), platformObj.getObject("location").getFloat("y"));
				pe->addBody(_platform, platformObj.getObject("bodyPadding").getFloat("x"), platformObj.getObject("bodyPadding").getFloat("y"));
				_game->getWorld()->addGameObject(_platform);
				de->load(_platform, R"(assets\levels\level1\platform)");
			}

			de->load("background", R"(assets\levels\level1\background)");
			//  de->loadText("escape_text", "PRESS 'ESC' TO RETURN TO THE MAINMENU", { 255, 255, 255 }, "arial", 18);
			_game->getEngine()->getAudioEngine()->load("level1_bgm", R"(assets/sounds/bgm/level1_bgm.mp3)", AUDIOTYPE::MUSIC);
			_bullets = new std::vector<MovablePlatform*>;

			loadCharacters(levelObj.getArray("startingPositions"));

			loadBulletBobs(levelObj.getArray("bobs"));

			// Load fps text
			de->loadDynamicText("fps", { 255, 255, 255 }, "arial", 18);

			delete doc;
		}

		void LoadingState::loadCharacters(JSON::JSONArray &startingPositions) {
			std::string loadCharacters[] = { "fiat", "nivek" };
			_characters = new std::vector<Character*>(2);

			for (int i = 0; i < 2; i++) {
				int retries = 0;
				do{
					JSON::JSONObject &posObj = startingPositions.getObject(i);
					(*_characters)[i] = factories::CharacterFactory::create(loadCharacters[i], *_game, posObj.getFloat("x"), posObj.getFloat("y"));
					if (retries++ > 10){
						_isError = true;
						return;
					}
				} while ((*_characters)[i] == nullptr);
			}

			(*_characters)[1]->setDirection(MovableGameObject::Direction::LEFT);
			(*_characters)[1]->setSpawnDirection(MovableGameObject::Direction::LEFT);

			// Create a HUD for each player
			_huds = new std::vector<helperclasses::HUD*>();
			if (_characters->size() == 2)
			{
				HUD *hudPanda = new HUD(*(*_characters)[0], 10);
				_huds->push_back(hudPanda);

				HUD *hudNivek = new HUD(*(*_characters)[1], _game->getEngine()->getDrawEngine()->getWindowWidth() - 230 - 10);
				_huds->push_back(hudNivek);
			}

			/*
			for (int i = 0; i < _characters->size(); i++) {
			HUD *hud = new HUD(*(*_characters)[i], 245 * i + 15);
			_huds->push_back(hud);
			}
			*/
		}

		void LoadingState::loadBulletBobs(JSON::JSONArray &bobs) {
			
			_bullets = new std::vector<MovablePlatform*>(bobs.size());

			for (int i = 0; i < bobs.size(); i++) {
					JSON::JSONObject &bobObj = bobs.getObject(i);

					(*_bullets)[i] = new MovablePlatform();
					(*_bullets)[i]->setSize(bobObj.getObject("size").getFloat("width"), bobObj.getObject("size").getFloat("height"));
					(*_bullets)[i]->setLocation(bobObj.getObject("location").getFloat("x"), bobObj.getObject("location").getFloat("y"));
					(*_bullets)[i]->setStartLocation(b2Vec2(bobObj.getObject("location").getFloat("x"), bobObj.getObject("location").getFloat("y")));
					(*_bullets)[i]->setEndLocation(b2Vec2(bobObj.getObject("endLocation").getFloat("x"), bobObj.getObject("endLocation").getFloat("y")));
					(*_bullets)[i]->setDirection(static_cast< MovableGameObject::Direction>((int)bobObj.getFloat("direction")));
					(*_bullets)[i]->setSpeed(bobObj.getObject("speed").getFloat("horizontal"), bobObj.getObject("speed").getFloat("vertical"));
					(*_bullets)[i]->setDieOnImpact(true);

					_game->getEngine()->getPhysicsEngine()->addKinematicBody((*_bullets)[i]);
					_game->getEngine()->getDrawEngine()->loadMap((*_bullets)[i], MovableGameObject::State::IDLE, R"(assets\levels\level2\bullet.sprite)", 1097, 494, 0.1);
			}

			/*
			_bullets = new std::vector<MovablePlatform*>(1);
			(*_bullets)[0] = new MovablePlatform();
			(*_bullets)[0]->setSize(110, 50);
			(*_bullets)[0]->setLocation(-1000, 550);
			(*_bullets)[0]->setStartLocation(b2Vec2(-1000, 550));
			(*_bullets)[0]->setEndLocation(b2Vec2(2700, 550));
			(*_bullets)[0]->setDirection(MovableGameObject::Direction::RIGHT);
			(*_bullets)[0]->setSpeed(20.0f, 0.0f);
			(*_bullets)[0]->setDieOnImpact(true);
			_game->getEngine()->getPhysicsEngine()->addKinematicBody((*_bullets)[0]);
			_game->getEngine()->getDrawEngine()->loadMap((*_bullets)[0], MovableGameObject::State::IDLE, R"(assets\levels\level2\bullet.sprite)", 1097, 494, 0.1);
			*/
		}


		void LoadingState::loadKeybindings() {

			try{
				ConfigManager &manager = ConfigManager::getInstance();
				InputDeviceBinding *binding = new InputDeviceBinding();

				for (int i = 0; i < 2; i++)
				{
					binding->setKeyBinding(manager.getKey(i, "walkRight"), new actions::RightWalkAction((*_characters)[i]));
					binding->setKeyBinding(manager.getKey(i, "walkLeft"), new actions::LeftWalkAction((*_characters)[i]));
					binding->setKeyBinding(manager.getKey(i, "jump"), new actions::JumpAction((*_characters)[i]));
					binding->setKeyBinding(manager.getKey(i, "roll"), new actions::RollAction((*_characters)[i]));
					binding->setKeyBinding(manager.getKey(i, "midrange"), new actions::MidRangeAttackAction((*_characters)[i]));
				}

				_game->getEngine()->getInputEngine()->setDeviceBinding("keyboard", binding);
			}
			catch (...)
			{
				std::cout << "Cannot load keyboard bindings.";
			}
		}

		void LoadingState::loadTutorial() {
			DrawEngine *de = _game->getEngine()->getDrawEngine();

			//de->loadText("tutIntro", "Welcome to the S.D.M.G. tutorial!", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutIntro", "Welcome! We will start by learning basic movement, press enter to continue", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutNivek1", "We shall begin with Nivek, Press left arrow key (<-) to move left", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutNivek2", "Press right arrow key (->) to move right", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutNivek3", "Press up arrow key (^) to jump", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutNivek4", "We will now learn attacking movements, press enter to continue", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutNivek5", "To perform a close range attack, press 'L'", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutNivek6", "To dodge an enemy attack, execute a roll, press 'K'", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutFiat1", "Now we shall explain Fiat's movements, press enter to continue", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutFiat2", "Press 'A' to move left", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutFiat3", "Press 'D' to move right", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutFiat4", "Press 'W' to jump", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutFiat5", "We will now learn attacking movements, press enter to continue", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutFiat6", "To perform a close range attack, press the 'Q' key", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutFiat7", "To dodge an enemy attack, execute a roll, press 'R'", { 255, 255, 255 }, "arial", 30);
			de->loadText("tutEnd", "You have successfully passed the tutorial, you are ready to play the game, press 'Esc' to quit!", { 255, 255, 255 }, "arial", 30);
		}

		void LoadingState::loadStatic() {
			PhysicsEngine *pe = _game->getEngine()->getPhysicsEngine();
			pe->setWorldGravity(0.0f, 100.0f);
			_platform = new model::Platform(false);
			_platform->setSize(1091, 94);
			_platform->setLocation(80 + 1091 / 2, 616 + 94 / 2);
			pe->addBody(_platform, 30, 20);
			_game->getWorld()->addGameObject(_platform);



			_game->getEngine()->getAudioEngine()->load("level1_bgm", R"(assets/sounds/bgm/level1_bgm.mp3)", AUDIOTYPE::MUSIC);

			//loadCharacters();


			DrawEngine *de = _game->getEngine()->getDrawEngine();

			if (false)
			{
				if (!_isTutorial) {
					_bullets = new std::vector<MovablePlatform*>(3);
					(*_bullets)[0] = new MovablePlatform();
					(*_bullets)[0]->setSize(110, 50);
					(*_bullets)[0]->setLocation(-1000, 550);
					(*_bullets)[0]->setStartLocation(b2Vec2(-1000, 550));
					(*_bullets)[0]->setEndLocation(b2Vec2(2700, 550));
					(*_bullets)[0]->setDirection(MovableGameObject::Direction::RIGHT);
					(*_bullets)[0]->setSpeed(20.0f, 0.0f);
					(*_bullets)[0]->setDieOnImpact(true);
					pe->addKinematicBody((*_bullets)[0]);

					(*_bullets)[1] = new MovablePlatform();
					(*_bullets)[1]->setSize(110, 50);
					(*_bullets)[1]->setLocation(-1200, 300);
					(*_bullets)[1]->setStartLocation(b2Vec2(-1200, 300));
					(*_bullets)[1]->setEndLocation(b2Vec2(3000, 300));
					(*_bullets)[1]->setDirection(MovableGameObject::Direction::RIGHT);
					(*_bullets)[1]->setSpeed(10.0f, 0.0f);
					(*_bullets)[1]->setDieOnImpact(true);
					pe->addKinematicBody((*_bullets)[1]);

					(*_bullets)[2] = new MovablePlatform();
					(*_bullets)[2]->setSize(110, 50);
					(*_bullets)[2]->setLocation(-700, 475);
					(*_bullets)[2]->setStartLocation(b2Vec2(-700, 475));
					(*_bullets)[2]->setEndLocation(b2Vec2(2500, 475));
					(*_bullets)[2]->setDirection(MovableGameObject::Direction::RIGHT);
					(*_bullets)[2]->setSpeed(15.0f, 0.0f);
					(*_bullets)[2]->setDieOnImpact(true);
					pe->addKinematicBody((*_bullets)[2]);

					de->loadMap((*_bullets)[0], MovableGameObject::State::IDLE, R"(assets\levels\level1\bullet.sprite)", 1097, 494, 0.1);
					de->loadMap((*_bullets)[1], MovableGameObject::State::IDLE, R"(assets\levels\level1\bullet.sprite)", 1097, 494, 0.1);
					de->loadMap((*_bullets)[2], MovableGameObject::State::IDLE, R"(assets\levels\level1\bullet.sprite)", 1097, 494, 0.1);
				}
			}
			else
				_bullets = new std::vector<MovablePlatform*>(0);

			/*    Kinematic Bodies
			model::MovablePlatform *mpVer = new model::MovablePlatform();
			mpVer->setSize(100, 30);
			mpVer->setStartLocation(b2Vec2(700, 200));
			mpVer->setEndLocation(b2Vec2(700, 500));
			mpVer->setDirection(MovableGameObject::Direction::UP);
			mpVer->setSpeed(0.0f, 10.0f);
			pe->addKinematicBody(mpVer);
			*/

			//  pe->resume();

			de->load(_platform, R"(assets\levels\level1\platform)");

			de->load("background", R"(assets\levels\level1\background)");
			de->loadText("escape_text", "PRESS 'ESC' TO RETURN TO THE MAINMENU", { 255, 255, 255 }, "arial", 18);

			InputDeviceBinding *binding = new InputDeviceBinding();
			binding->setKeyBinding(SDLK_RIGHT, new actions::RightWalkAction((*_characters)[0]));
			binding->setKeyBinding(SDLK_LEFT, new actions::LeftWalkAction((*_characters)[0]));
			binding->setKeyBinding(SDLK_UP, new actions::JumpAction((*_characters)[0]));
			binding->setKeyBinding(SDLK_KP_0, new actions::RollAction((*_characters)[0]));
			binding->setKeyBinding(SDLK_l, new actions::MidRangeAttackAction((*_characters)[0]));
			// Voor controller
			binding->setKeyBinding(0, new actions::JumpAction((*_characters)[0]));
			binding->setKeyBinding(2, new actions::LeftWalkAction((*_characters)[0]));
			binding->setKeyBinding(3, new actions::RightWalkAction((*_characters)[0]));
			binding->setKeyBinding(10, new actions::JumpAction((*_characters)[0]));
			binding->setKeyBinding(9, new actions::RollAction((*_characters)[0]));
			binding->setKeyBinding(8, new actions::RollAction((*_characters)[0]));
			binding->setKeyBinding(12, new actions::MidRangeAttackAction((*_characters)[0]));


			binding->setKeyBinding(SDLK_d, new actions::RightWalkAction((*_characters)[1]));
			binding->setKeyBinding(SDLK_a, new actions::LeftWalkAction((*_characters)[1]));
			binding->setKeyBinding(SDLK_w, new actions::JumpAction((*_characters)[1]));
			binding->setKeyBinding(SDLK_r, new actions::RollAction((*_characters)[1]));
			binding->setKeyBinding(SDLK_q, new actions::MidRangeAttackAction((*_characters)[1]));

			_game->getEngine()->getInputEngine()->setDeviceBinding("keyboard", binding);



			/*
			// Voor panda
			InputDeviceBinding *binding2 = new InputDeviceBinding();
			binding2->setKeyBinding(SDLK_d, new actions::RightWalkAction((*_characters)[1]));
			binding2->setKeyBinding(SDLK_a, new actions::LeftWalkAction((*_characters)[1]));
			binding2->setKeyBinding(SDLK_w, new actions::JumpAction((*_characters)[1]));
			binding2->setKeyBinding(SDLK_r, new actions::RollAction((*_characters)[1]));
			binding2->setKeyBinding(SDLK_q, new actions::MidRangeAttackAction((*_characters)[1]));
			_game->getEngine()->getInputEngine()->setDeviceBinding("fiat", binding2);
			*/



			/*
			// Voor panda
			InputDeviceBinding *binding2 = new InputDeviceBinding();
			binding2->setKeyBinding(SDLK_d, new actions::RightWalkAction((*_characters)[1]));
			binding2->setKeyBinding(SDLK_a, new actions::LeftWalkAction((*_characters)[1]));
			binding2->setKeyBinding(SDLK_w, new actions::JumpAction((*_characters)[1]));
			binding2->setKeyBinding(SDLK_r, new actions::RollAction((*_characters)[1]));
			binding2->setKeyBinding(SDLK_q, new actions::MidRangeAttackAction((*_characters)[1]));
			_game->getEngine()->getInputEngine()->setDeviceBinding("fiat", binding2);
			*/

			_isLoaded = true;
		}
	}
}