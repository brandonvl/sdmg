//
//
//  @ Project : SDMG
//  @ File Name : IntroState.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#include "IntroState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "LoadingState.h"
#include "engine\physics\PhysicsEngine.h"
#include "model\Platform.h"
#include "model\Character.h"

namespace sdmg {
	namespace gamestates {

		void IntroState::init(GameBase &game)
		{
			PhysicsEngine *pe = game.getEngine()->getPhysicsEngine();
			pe->setWorldGravity(0.0f, 100.0f);
			_platform = new model::Platform();
			_platform->setSize(1091, 94);
			pe->addBody(80 + 1091 / 2, 616 + 94 / 2, 1091, 94 - 15, false, _platform);

			_character = new model::Character();
			_character->setSize(110.06, 143.25);
			_character->setDirection(GameObject::Direction::RIGHT);
			_character->setSpeed(20, 1);//pe->getWorldGravity().y / 30);
			pe->addBody(100 + 110 / 2, 10, 110, 143, true, _character);

			pe->resume();
			
			DrawEngine *de = game.getEngine()->getDrawEngine();

			de->loadMap(_character, GameObject::State::WALKING, R"(assets\nivek\walking.png)", 440.25, 573, .25);
			de->loadMap(_character, GameObject::State::IDLE, R"(assets\nivek\idle.png)", 393.125, 548, .25);
			de->loadMap(_character, GameObject::State::JUMPING, R"(assets\nivek\jumping.png)", 462.2, 622, .25);
			de->loadMap(_character, GameObject::State::FORWARD_ROLL, R"(assets\nivek\forward_roll.png)", 761.14, 608, .25);
			
			de->load(_platform, R"(assets\platform.png)");
			de->load("background", R"(assets\background.png)");
			std::cout << "Initing IntroState ... " << std::endl;
		}

		void IntroState::cleanup(GameBase &game)
		{
			std::cout << "Cleaning up IntroState ... " << std::endl;
			game.getEngine()->getDrawEngine()->unload("surprise");
		}

		void IntroState::pause(GameBase &game)
		{
			std::cout << "Pausing IntroState ... " << std::endl;
		}

		void IntroState::resume(GameBase &game)
		{
			std::cout << "Resuming IntroState ... " << std::endl;
		}

		void IntroState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			//std::cout << "Handling events IntroState ... " << std::endl;
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
					case SDLK_1:
						std::cout << "Key 1 pressed. Switching State.. " << std::endl;
						changeState(game, LoadingState::getInstance());
						break;
					case SDLK_LEFT:
						_character->setDirection(GameObject::Direction::LEFT);
						_character->setState(GameObject::State::WALKING);
						game.getEngine()->getPhysicsEngine()->doAction(_character, PhysicsEngine::Action::MOVELEFT);
						break;
					case SDLK_RIGHT:
						_character->setDirection(GameObject::Direction::RIGHT);
						_character->setState(GameObject::State::WALKING);
						game.getEngine()->getPhysicsEngine()->doAction(_character, PhysicsEngine::Action::MOVERIGHT);
						break;
					case SDLK_SPACE:
						if (_character->getState() != GameObject::State::JUMPING) {
							_character->setState(GameObject::State::JUMPING);
							game.getEngine()->getPhysicsEngine()->doAction(_character, PhysicsEngine::Action::JUMP);
						}
						break;
					case SDLK_r:
						_character->setState(GameObject::State::FORWARD_ROLL);
						game.getEngine()->getPhysicsEngine()->doAction(_character, PhysicsEngine::Action::MOVERIGHT);
						break;
					}
				}

				if (event.type == SDL_KEYUP) {
					switch (event.key.keysym.sym)
					{
					case SDLK_LEFT:
					case SDLK_RIGHT:
					case SDLK_SPACE:
						_character->setState(GameObject::State::IDLE);
						game.getEngine()->getPhysicsEngine()->doAction(_character, PhysicsEngine::Action::IDLE);
						break;
					}
				}
			}
		}

		void IntroState::update(GameBase &game, GameTime &gameTime)
		{
			//std::cout << "Updating IntroState ... " << std::endl;

			game.getEngine()->getPhysicsEngine()->update();
		}

		void IntroState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();

			game.getEngine()->getDrawEngine()->draw("background");
			//game.getEngine()->getDrawEngine()->drawBodies(game.getEngine()->getPhysicsEngine()->getBodyList());
			game.getEngine()->getDrawEngine()->draw(_platform, (_platform->getX() * 20.0f) - 1091 / 2, (_platform->getY() * 20.0f) - 94 / 2 - 15);
			game.getEngine()->getDrawEngine()->draw(_character, _character->getState(), _character->getDirection(), (_character->getX() * 20.0f) - 110.06 / 2, (_character->getY() * 20.0f) - 142.25 / 2, gameTime.getTotalMilisecondsRunning() / 100);

			game.getEngine()->getDrawEngine()->render();
		}
		
	}
}
