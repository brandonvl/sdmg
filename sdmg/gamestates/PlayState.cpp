//
//
//  @ Project : SDMG
//  @ File Name : PlayState.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "PlayState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "LoadingState.h"
#include "engine\physics\PhysicsEngine.h"
#include "model\Platform.h"
#include "model\MovablePlatform.h"
#include "model\Character.h"
#include "factories\CharacterFactory.h"
#include "engine\input\InputEngine.h"
#include "actions\Actions.h"

namespace sdmg {
	namespace gamestates {
		void PlayState::init(GameBase &game)
		{
			PhysicsEngine *pe = game.getEngine()->getPhysicsEngine();
			pe->setWorldGravity(0.0f, 100.0f);
			_platform = new model::Platform();
			_platform->setSize(1091, 94);
			_platform->setLocation(80 + 1091 / 2, 616 + 94 / 2);
			pe->addBody(_platform, 30, 20);

			_characters[0] = factories::CharacterFactory::create("nivek", game);
			_characters[1] = factories::CharacterFactory::create("nivek", game);
			
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

			DrawEngine *de = game.getEngine()->getDrawEngine();
			de->load(_platform, R"(assets\platform.png)");
			de->load("background", R"(assets\background.png)");

			InputDeviceBinding *binding = new InputDeviceBinding();
			binding->setKeyBinding(SDLK_RIGHT, new actions::RightWalkAction(_characters[0]));
			binding->setKeyBinding(SDLK_LEFT, new actions::LeftWalkAction(_characters[0]));
			binding->setKeyBinding(SDLK_UP, new actions::JumpAction(_characters[0]));
			binding->setKeyBinding(SDLK_KP_0, new actions::RollAction(_characters[0]));


			binding->setKeyBinding(SDLK_d, new actions::RightWalkAction(_characters[1]));
			binding->setKeyBinding(SDLK_a, new actions::LeftWalkAction(_characters[1]));
			binding->setKeyBinding(SDLK_SPACE, new actions::JumpAction(_characters[1]));
			binding->setKeyBinding(SDLK_r, new actions::RollAction(_characters[1]));
			game.getEngine()->getInputEngine()->setDeviceBinding("keyboard", binding);
		}

		void PlayState::cleanup(GameBase &game)
		{
			std::cout << "Cleaning up IntroState ... " << std::endl;
			game.getEngine()->getDrawEngine()->unload("surprise");
		}

		void PlayState::pause(GameBase &game)
		{
			std::cout << "Pausing IntroState ... " << std::endl;
		}

		void PlayState::resume(GameBase &game)
		{
			std::cout << "Resuming IntroState ... " << std::endl;
		}

		void PlayState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				switch (event.type) {
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					game.getEngine()->getInputEngine()->handleEvent(event);
					break;
				case SDL_QUIT:
					game.stop();
					break;
				}
			}
		}

		void PlayState::update(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getInputEngine()->runActions(game);
			game.getEngine()->getDrawEngine()->update();
			game.getEngine()->getPhysicsEngine()->update();
		}

		void PlayState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();

			game.getEngine()->getDrawEngine()->draw("background");
			//game.getEngine()->getDrawEngine()->drawBodies(game.getEngine()->getPhysicsEngine()->getBodyList());
			game.getEngine()->getDrawEngine()->draw(_platform);
			game.getEngine()->getDrawEngine()->drawSlice(_characters[0]);
			game.getEngine()->getDrawEngine()->drawSlice(_characters[1]);

			//game.getEngine()->getDrawEngine()->drawText("SDMG!", Rectangle(99, 214, 100, 50));

			game.getEngine()->getDrawEngine()->render();
		}

		
	}
}
