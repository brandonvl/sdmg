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

namespace sdmg {
	namespace gamestates {

		void LoadingState::init(GameBase &game)
		{
			_game = &game;
			_game->getWorld()->clearWorld();
			
			_isLoaded = false;
			_isError = false;

			game.getEngine()->getDrawEngine()->load("loading", R"(assets\screens\loadingscreen)");

			SDL_Thread *thread;
			int         threadReturnValue;

			printf("\nSimple SDL_CreateThread test:");

			// Simply create a thread
			thread = SDL_CreateThread(loadThread, "LoadThread", (void *)this);
			//SDL_WaitThread(thread, NULL);
			//SDL_DetachThread(thread);

		}

		void LoadingState::cleanup(GameBase &game)
		{
			game.getEngine()->getDrawEngine()->unload("loading");
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
				PlayState::getInstance().setCharacters(_characters);
				PlayState::getInstance().setPlatform(_platform);
				PlayState::getInstance().setBullets(_bullets);
				changeState(game, PlayState::getInstance());
			}
			if (_isError) {
				// Clean uppen
				changeState(game, MainMenuState::getInstance());
			}
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
			PhysicsEngine *pe = _game->getEngine()->getPhysicsEngine();
			pe->setWorldGravity(0.0f, 100.0f);
			_platform = new model::Platform();
			_platform->setSize(1091, 94);
			_platform->setLocation(80 + 1091 / 2, 616 + 94 / 2);
			pe->addBody(_platform, 30, 20);
			_game->getWorld()->addGameObject(_platform);

			_characters = new std::vector<Character*>(2);

			_game->getEngine()->getAudioEngine()->load("level1_bgm", R"(assets/sounds/bgm/level1_bgm.mp3)", AUDIOTYPE::MUSIC);

			int retries = 0;

			do{
				(*_characters)[0] = factories::CharacterFactory::create("nivek", *_game, 1100, -100);
				if (retries++ > 10){
					_isError = true;
					return;
				}
			} while ((*_characters)[0] == nullptr);
			
			(*_characters)[0]->setDirection(MovableGameObject::Direction::LEFT);
			(*_characters)[0]->setSpawnDirection(MovableGameObject::Direction::LEFT);

			do{
				(*_characters)[1] = factories::CharacterFactory::create("fiat", *_game, 150, -100);
				if (retries++ > 10){
					_isError = true;
					return;
				}
			} while ((*_characters)[1] == nullptr);
			
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

			DrawEngine *de = _game->getEngine()->getDrawEngine();
			de->load(_platform, R"(assets\levels\level1\platform)");
			de->loadMap((*_bullets)[0], MovableGameObject::State::IDLE, R"(assets\levels\level1\bullet.sprite)", 1097, 494, 0.1);
			de->loadMap((*_bullets)[1], MovableGameObject::State::IDLE, R"(assets\levels\level1\bullet.sprite)", 1097, 494, 0.1);
			de->loadMap((*_bullets)[2], MovableGameObject::State::IDLE, R"(assets\levels\level1\bullet.sprite)", 1097, 494, 0.1);
			de->load("background", R"(assets\levels\level1\background)");
			de->loadText("escape_text", "PRESS 'ESC' TO RETURN TO THE MAINMENU", { 255, 255, 255 }, "arial", 18);

			InputDeviceBinding *binding = new InputDeviceBinding();
			binding->setKeyBinding(SDLK_RIGHT, new actions::RightWalkAction((*_characters)[0]));
			binding->setKeyBinding(SDLK_LEFT, new actions::LeftWalkAction((*_characters)[0]));
			binding->setKeyBinding(SDLK_UP, new actions::JumpAction((*_characters)[0]));
			binding->setKeyBinding(SDLK_KP_0, new actions::RollAction((*_characters)[0]));
			//  binding->setKeyBinding(SDLK_KP_1, new actions::RespawnAction((*_characters)[0]));

			binding->setKeyBinding(SDLK_d, new actions::RightWalkAction((*_characters)[1]));
			binding->setKeyBinding(SDLK_a, new actions::LeftWalkAction((*_characters)[1]));
			binding->setKeyBinding(SDLK_w, new actions::JumpAction((*_characters)[1]));
			binding->setKeyBinding(SDLK_r, new actions::RollAction((*_characters)[1]));
			//  binding->setKeyBinding(SDLK_q, new actions::RespawnAction((*_characters)[1]));
			_game->getEngine()->getInputEngine()->setDeviceBinding("keyboard", binding);

			binding->setKeyBinding(0, new actions::JumpAction((*_characters)[1]));
			binding->setKeyBinding(2, new actions::LeftWalkAction((*_characters)[1]));
			binding->setKeyBinding(3, new actions::RightWalkAction((*_characters)[1]));
			binding->setKeyBinding(10, new actions::JumpAction((*_characters)[1]));
			binding->setKeyBinding(11, new actions::RollAction((*_characters)[1]));

			_isLoaded = true;
		}
	}
}
