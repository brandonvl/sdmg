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
			pe->addBody(_platform, 30, 35);

			_character = factories::CharacterFactory::create("nivek", game);

			pe->resume();

			DrawEngine *de = game.getEngine()->getDrawEngine();
			de->load(_platform, R"(assets\platform.png)");
			de->load("background", R"(assets\background.png)");

			InputDeviceBinding *binding = new InputDeviceBinding();
			binding->setKeyBinding(SDLK_RIGHT, new actions::RightWalkAction(_character));
			binding->setKeyBinding(SDLK_LEFT, new actions::LeftWalkAction(_character));
			binding->setKeyBinding(SDLK_SPACE, new actions::JumpAction(_character));
			binding->setKeyBinding(SDLK_r, new actions::RollAction(_character));
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

			//std::cout << "Updating IntroState ... " << std::endl;
			game.getEngine()->getPhysicsEngine()->update();
		}

		void PlayState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();

			game.getEngine()->getDrawEngine()->draw("background");
			game.getEngine()->getDrawEngine()->drawBodies(game.getEngine()->getPhysicsEngine()->getBodyList());
			game.getEngine()->getDrawEngine()->draw(_platform);
			game.getEngine()->getDrawEngine()->draw(_character, gameTime.getTotalMilisecondsRunning() / 100);

			//game.getEngine()->getDrawEngine()->drawText("SDMG!", Rectangle(99, 214, 100, 50));

			game.getEngine()->getDrawEngine()->render();
		}

		
	}
}
