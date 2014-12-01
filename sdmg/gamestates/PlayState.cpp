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

#include "engine\particle\ParticleEngine.h"

namespace sdmg {
	namespace gamestates {
		void PlayState::init(GameBase &game)
		{
			_game = &game;
			_editor = new Editor(game);
			game.getEngine()->getPhysicsEngine()->resume();
			game.getEngine()->getAudioEngine()->play("bgm", 0);

			_step = 1.0f / 4.0f;
			_lastUpdate = std::chrono::high_resolution_clock::now();
		}

		void PlayState::setHUDs(std::vector<helperclasses::HUD *> *huds)
		{
			_huds = huds;
		}

		void PlayState::cleanup(GameBase &game)
		{
			//game.getEngine()->getPhysicsEngine()->cleanUp();
			//game.getEngine()->getDrawEngine()->unloadAll();
			//game.getEngine()->getAudioEngine()->unloadAll();

			/*if (_huds) {
				for (auto it : *_huds) {
					delete it;
				}
				_huds->clear();
			}

			delete _huds;
			_huds = nullptr;*/
		}

		void PlayState::pause(GameBase &game)
		{
			std::cout << "Pausing IntroState ... " << std::endl;
		}

		void PlayState::resume(GameBase &game)
		{
			game.getEngine()->getAudioEngine()->play("bgm", 0);
			// std::cout << "Resuming IntroState ... " << std::endl;
		}

		void PlayState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				game.getEngine()->getInputEngine()->handleEvent(event);

				if (!game.getEngine()->getInputEngine()->handleControllers(event)) {
					switch (event.type) {
					case SDL_KEYDOWN:
						switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							//changeState(game, MainMenuState::getInstance());
							break;
						case SDLK_F1:
							if (!event.key.repeat)
								_showFPS = !_showFPS;
							break;
						case SDLK_F2:
							if (!event.key.repeat)
								//_showHitBoxes = !_showHitBoxes;
								_showHitBoxes = true;
							break;
						case SDLK_F3:
							if (!event.key.repeat)
								//_showHitBoxes = !_showHitBoxes;
								_showClickBoxes = true;
							break;
						case SDLK_F4:
							if (!event.key.repeat){
								//_editor->toggle();
							}
						}

						break;
					case SDL_QUIT:
						if (_huds) {
							for (auto it : *_huds) {
								delete it;
							}
							_huds->clear();
						}

						delete _huds;
						_huds = nullptr;

						game.stop();
						break;
					}
				}
			}
		}

		void PlayState::update(GameBase &game, GameTime &gameTime)
		{
			if (!_editor->isEnabled()) {
				if (game.getWorld()->isGameOver()) {
					if (game.getWorld()->getAliveList().size() > 0)
						game.getWorld()->getAliveList()[0]->die();
					game.getEngine()->getPhysicsEngine()->pause();
					changeState(game, GameOverState::getInstance());
				}

				if (_showFPS)
					_fps = game.getFPS() == _fps ? _fps : game.getFPS();


				game.getEngine()->getInputEngine()->update(game);
				game.getEngine()->getDrawEngine()->update();
				game.getEngine()->getPhysicsEngine()->update();

				auto curTime = std::chrono::high_resolution_clock::now();
				float diff = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - _lastUpdate).count() / 1000.0f;

				_lastUpdate = curTime;
				_accumulator += diff;

				while (_accumulator > _step) {
					for (auto obj : game.getWorld()->getPlayers())
					{
						obj->addPP(1);
					}
					_accumulator -= _step;
				}

				game.getEngine()->getInputEngine()->update(game);
				game.getEngine()->getDrawEngine()->update();
				game.getEngine()->getPhysicsEngine()->update();
			}
		}

		void PlayState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			preformDraw(game);
			game.getEngine()->getDrawEngine()->render();
		}

		void PlayState::preformDraw(GameBase &game) {
			DrawEngine *de = game.getEngine()->getDrawEngine();
			de->draw("background");

			if (_showHitBoxes)
				de->drawBodies(game.getEngine()->getPhysicsEngine()->getBodyList());

			if (_showClickBoxes)
				de->drawHitBoxes(game.getEngine()->getInputEngine()->getMouse().getClickBoxes());

			for (auto obj : game.getWorld()->getGameObjects())
				de->draw(obj);
			
			if (_huds && !_editor->isEnabled()) {
				for (helperclasses::HUD *hud : *_huds) {
					hud->draw(*de);
				}
			}

			if (_showFPS)
				de->drawDynamicText("fps", "FPS: " + std::to_string(_fps), game.getEngine()->getDrawEngine()->getWindowWidth() - 100, 10);

			_editor->draw();
		}
	}
}
