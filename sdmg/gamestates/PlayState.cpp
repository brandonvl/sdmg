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
#include "engine\audio\AudioEngine.h"
#include "helperclasses\HUD.h"

namespace sdmg {
	namespace gamestates {
		void PlayState::init(GameBase &game)
		{
			game.getEngine()->getPhysicsEngine()->resume();
			game.getEngine()->getAudioEngine()->play("level1_bgm", 0);
		}

		void PlayState::setCharacters(std::vector<model::Character*> *characters)
		{
			_characters = characters;
		}

		void PlayState::setPlatform(model::Platform *platform)
		{
			_platform = platform;
		}

		void PlayState::setBullets(std::vector<model::MovablePlatform *> *bullets)
		{
			_bullets = bullets;
		}

		void PlayState::setHUDs(std::vector<helperclasses::HUD *> *huds)
		{
			_huds = huds;
		}

		void PlayState::cleanup(GameBase &game)
		{
			game.getEngine()->getPhysicsEngine()->cleanUp();
			game.getEngine()->getDrawEngine()->unloadAll();
			game.getEngine()->getAudioEngine()->unloadAll();
			//  game.getWorld()->clearWorld();

			/*
			for (MovablePlatform *platform : *_bullets)
				delete platform;*/

			if (_huds) {
				for (auto it : *_huds) {
					delete it;
				}
				_huds->clear();
			}

			delete _huds;
			delete _characters;
			delete _bullets;
			//delete _platform;

			_bullets = nullptr;
			_huds = nullptr;
			_characters = nullptr;
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
				if (!game.getEngine()->getInputEngine()->handleControllers(event)) {
					switch (event.type) {
					case SDL_KEYDOWN:
					case SDL_KEYUP:
						switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							changeState(game, MainMenuState::getInstance());
							break;
						case SDLK_F1:
							if (!event.key.repeat)
								_showFPS = !_showFPS;
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
		}

		void PlayState::update(GameBase &game, GameTime &gameTime)
		{
			if (game.getWorld()->isGameOver()) {
				if (game.getWorld()->getAliveList().size() > 0)
					game.getWorld()->getAliveList()[0]->die();
				game.getEngine()->getPhysicsEngine()->pause();
				//changeState(game, GameOverState::getInstance());
				game.getStateManager()->pushState(GameOverState::getInstance());
			}

			if (_showFPS)
				_fps = game.getFPS() == _fps ? _fps : game.getFPS();

			game.getEngine()->getInputEngine()->runActions(game);
			game.getEngine()->getDrawEngine()->update();
			game.getEngine()->getPhysicsEngine()->update();
		}

		void PlayState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();

			game.getEngine()->getDrawEngine()->draw("background");
			//game.getEngine()->getDrawEngine()->drawBodies(game.getEngine()->getPhysicsEngine()->getBodyList());

			for (int i = 0; i < _bullets->size(); i++)
				game.getEngine()->getDrawEngine()->drawSlice((*_bullets)[i]);

			game.getEngine()->getDrawEngine()->draw(_platform);
			// Deze heb ik ook uitgecomment in de LoadState
			//  game.getEngine()->getDrawEngine()->drawText("escape_text", 10, 10);

			for (int i = 0; i < _characters->size(); i++)
				game.getEngine()->getDrawEngine()->drawSlice((*_characters)[i]);

			for (helperclasses::HUD *hud : *_huds) {
				hud->draw(*game.getEngine()->getDrawEngine());
			}

			if (_showFPS)
				game.getEngine()->getDrawEngine()->drawDynamicText("fps", "FPS: " + std::to_string(_fps), game.getEngine()->getDrawEngine()->getWindowWidth() - 100, 10);

			game.getEngine()->getDrawEngine()->render();
		}
	}
}
