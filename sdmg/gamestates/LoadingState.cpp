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

namespace sdmg {
	namespace gamestates {

		void LoadingState::init(GameBase &game)
		{
			_game = &game;
			_isLoaded = false;

			game.getEngine()->getDrawEngine()->load("loading", R"(assets\loadscreen\loading.png)");
			std::cout << "Initing LoadingState ... " << std::endl;


			SDL_Thread *thread;
			int         threadReturnValue;

			printf("\nSimple SDL_CreateThread test:");

			// Simply create a thread
			thread = SDL_CreateThread(loadThread, "LoadThread", (void *)this);
		}

		void LoadingState::cleanup(GameBase &game)
		{
			//game.getEngine()->getDrawEngine()->unload("cowboy");
			//std::cout << "Cleaning up LoadingState ... " << std::endl;
		}

		void LoadingState::pause(GameBase &game)
		{
			std::cout << "Pausing LoadingState ... " << std::endl;
		}

		void LoadingState::resume(GameBase &game)
		{
			std::cout << "Resuming LoadingState ... " << std::endl;
		}

		void LoadingState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			//std::cout << "Handling events LoadingState ... " << std::endl;
			SDL_Event event;
			if (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					game.stop();
				}

				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						game.stop();
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
				changeState(game, PlayState::getInstance());
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

			_characters = new std::vector<Character*>(2);

			(*_characters)[0] = factories::CharacterFactory::create("nivek", *_game, 1100, -100);
			(*_characters)[0]->setDirection(MovableGameObject::Direction::LEFT);

			(*_characters)[1] = factories::CharacterFactory::create("fiat", *_game, 150, -100);

			/*    Kinematic Bodies
			model::MovablePlatform *mpHor = new model::MovablePlatform();
			mpHor->setSize(100, 30);
			mpHor->setStartLocation(b2Vec2(300, 200));
			mpHor->setEndLocation(b2Vec2(600, 200));
			mpHor->setDirection(MovableGameObject::Direction::RIGHT);
			mpHor->setSpeed(10.0f, 0.0f);
			pe->addKinematicBody(mpHor);


			model::MovablePlatform *mpVer = new model::MovablePlatform();
			mpVer->setSize(100, 30);
			mpVer->setStartLocation(b2Vec2(700, 200));
			mpVer->setEndLocation(b2Vec2(700, 500));
			mpVer->setDirection(MovableGameObject::Direction::UP);
			mpVer->setSpeed(0.0f, 10.0f);
			pe->addKinematicBody(mpVer);

			*/
			pe->resume();

			DrawEngine *de = _game->getEngine()->getDrawEngine();
			de->load(_platform, R"(assets\platform.png)");
			de->load("background", R"(assets\background.png)");

			InputDeviceBinding *binding = new InputDeviceBinding();
			binding->setKeyBinding(SDLK_RIGHT, new actions::RightWalkAction((*_characters)[0]));
			binding->setKeyBinding(SDLK_LEFT, new actions::LeftWalkAction((*_characters)[0]));
			binding->setKeyBinding(SDLK_UP, new actions::JumpAction((*_characters)[0]));
			binding->setKeyBinding(SDLK_KP_0, new actions::RollAction((*_characters)[0]));
			binding->setKeyBinding(SDLK_KP_1, new actions::RespawnAction((*_characters)[0]));


			binding->setKeyBinding(SDLK_d, new actions::RightWalkAction((*_characters)[1]));
			binding->setKeyBinding(SDLK_a, new actions::LeftWalkAction((*_characters)[1]));
			binding->setKeyBinding(SDLK_SPACE, new actions::JumpAction((*_characters)[1]));
			binding->setKeyBinding(SDLK_r, new actions::RollAction((*_characters)[1]));
			binding->setKeyBinding(SDLK_q, new actions::RespawnAction((*_characters)[1]));
			_game->getEngine()->getInputEngine()->setDeviceBinding("keyboard", binding);

			_isLoaded = true;
		}
	}
}
