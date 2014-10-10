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
#include "MainMenuState.h"
#include "GameOverState.h"
#include "engine\World.h"

namespace sdmg {
	namespace gamestates {
		void PlayState::init(GameBase &game)
		{
		}

		void PlayState::setCharacters(std::vector<model::Character*> *characters)
		{
			_characters = characters;
		}

		void PlayState::setPlatform(model::Platform *platform)
		{
			_platform = platform;
		}

		void PlayState::cleanup(GameBase &game)
		{
			game.getEngine()->getPhysicsEngine()->cleanUp();
			game.getEngine()->getDrawEngine()->unloadAll();
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
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							changeState(game, MainMenuState::getInstance());
							break;
						default:
							game.getEngine()->getInputEngine()->handleEvent(event);
							break;
					}
					
					break;
				case SDL_QUIT:
					game.stop();
					break;
				}
			}
		}

		void PlayState::update(GameBase &game, GameTime &gameTime)
		{
			if (game.getWorld()->isGameOver()) {
				game.getWorld()->getAliveList()[0]->die();
				changeState(game, GameOverState::getInstance());
			}

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
			game.getEngine()->getDrawEngine()->drawText("escape_text", 10,10);
			game.getEngine()->getDrawEngine()->drawSlice((*_characters)[0]);
			game.getEngine()->getDrawEngine()->drawSlice((*_characters)[1]);

			//game.getEngine()->getDrawEngine()->drawText("SDMG!", Rectangle(99, 214, 100, 50));

			game.getEngine()->getDrawEngine()->render();
		}
	}
}
