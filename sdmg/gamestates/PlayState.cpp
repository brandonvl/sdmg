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
#include "GameOverState.h"
#include "PauseState.h"
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

			_multiplier = 1.0f;
			_step = 1.0f / 7.0f;
			_lastUpdate = std::chrono::high_resolution_clock::now();
			_canDie = true;
		}

		void PlayState::setHUDs(std::vector<helperclasses::HUD *> *huds)
		{
			_huds = huds;
		}

		void PlayState::cleanup(GameBase &game)
		{
			delete _editor;
			_editor = nullptr;
		}

		void PlayState::pause(GameBase &game)
		{
			_game->getEngine()->getPhysicsEngine()->pause();
		}

		void PlayState::resume(GameBase &game)
		{
			_game->getEngine()->getPhysicsEngine()->resume();
			_lastUpdate = std::chrono::high_resolution_clock::now();
		}

		void PlayState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				if (!_editor->isEnabled()) {
					game.getEngine()->getInputEngine()->handleEvent(event);

					if (!game.getEngine()->getInputEngine()->handleControllers(event)) {
						if (!event.key.repeat){
							switch (event.type) {
							case SDL_KEYDOWN:
								switch (event.key.keysym.sym) {
								case SDLK_ESCAPE:
									game.getStateManager()->pushState(PauseState::getInstance());
									break;
								case SDLK_F1:
									if (!event.key.repeat)
										_showFPS = !_showFPS;
									break;
								case SDLK_F2:
									if (!event.key.repeat)
										_showHitBoxes = !_showHitBoxes;
									break;
								case SDLK_F3:
									if (!event.key.repeat)
										_showClickBoxes = !_showClickBoxes;
									break;
								case SDLK_F4:
									if (!event.key.repeat){
										_editor->toggle();
									}
									break;
								case SDLK_HOME:
									if (!event.key.repeat){
										_multiplier = 1.0f;
									}
									break;
								case SDLK_PAGEUP:
									if (!event.key.repeat){
										if (_multiplier > 0.1f)
										{
											_multiplier = _multiplier - 0.1;
										}
									}
									break;
								case SDLK_PAGEDOWN:
									if (!event.key.repeat)
									{
										if (_multiplier < 1.5f)
										{
											_multiplier = _multiplier + 0.1;
										}
									}
									break;
								}
								break;
							case SDL_QUIT:
								
								break;
							}
						}
						else if (event.type == SDL_QUIT){
							if (_huds) {
								for (auto it : *_huds) {
									delete it;
								}
								_huds->clear();
							}
							game.stop();
						}
					}
				}
				else _editor->handleEvent(event);
			}
		}

		void PlayState::update(GameBase &game, GameTime &gameTime)
		{
			if (!_editor->isEnabled()) {
				if (_canDie && game.getWorld()->isGameOver()) {
					if (game.getWorld()->getAliveList().size() > 0)
						game.getWorld()->getAliveList()[0]->die();
					game.getEngine()->getPhysicsEngine()->pause();

					auto players = game.getWorld()->getPlayers();
					for (auto p : players)
					{
						p->destroyAttackBody();
						p->destroyShootBody(*game.getEngine());
					}

					changeState(game, GameOverState::getInstance());
				}


				_game->getEngine()->getPhysicsEngine()->setSpeed(_game->getEngine()->getPhysicsEngine()->getSpeed() * _multiplier);
				_game->getEngine()->getDrawEngine()->setSpeed(_game->getEngine()->getDrawEngine()->getSpeed() * _multiplier);

				if (_showFPS)
					_fps = game.getFPS() == _fps ? _fps : game.getFPS();

				// ------------------------------------------- PARTICLE TEST --------------------------------------
				if (!_particlesSet) {
					for (auto obj : game.getWorld()->getPlayers()) {
						game.getEngine()->getParticleEngine()->registerGameObject(obj);
					}

					game.getEngine()->getParticleEngine()->createParticleSet("hit", 3, 175, 175, 350, 350);
					_particlesSet = true;
				}
				// ------------------------------------------- PARTICLE TEST --------------------------------------

				game.getEngine()->getInputEngine()->update(game);
				game.getEngine()->getDrawEngine()->update();
				game.getEngine()->getPhysicsEngine()->update();

				auto curTime = std::chrono::high_resolution_clock::now();
				float diff = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - _lastUpdate).count() / 1000.0f;

				_lastUpdate = curTime;
				_accumulator += diff;

				for (auto it : game.getWorld()->getPlayers()) {
					it->update(&gameTime, &game);
				}

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
			performDraw(game);
			game.getEngine()->getDrawEngine()->render();
		}

		void PlayState::performDraw(GameBase &game) {
			DrawEngine *de = game.getEngine()->getDrawEngine();
			de->draw("background");

			if (_showHitBoxes)
				de->drawBodies(game.getEngine()->getPhysicsEngine()->getBodyList());

			if (_showClickBoxes)
				de->drawHitBoxes(game.getEngine()->getInputEngine()->getMouse().getClickBoxes());

			for (auto obj : game.getWorld()->getPlatforms())
				de->draw(obj);

			for (auto obj : game.getWorld()->getPlayers())
				de->draw(obj);
			
			if (_huds && !_editor->isEnabled()) {
				for (helperclasses::HUD *hud : *_huds) {
					hud->draw(*de);
				}
			}

			if (_showFPS)
				game.getEngine()->getDrawEngine()->drawDynamicText("fps", "FPS: " + std::to_string(_fps), game.getEngine()->getDrawEngine()->getWindowWidth() - 100, 10);
			
			if (_multiplier != 1.0f) {
				std::string result;
				float temp = _multiplier * 100.0f;
				if (_multiplier > 1.0f)
				{
					temp = 100.0f - (temp - 100.0f);
				}
				else
				{
					temp = 100.0f + (100.0f - temp);
				}

				result = std::to_string(temp + .05);
				result = result.substr(0, result.size() - 7);
				result = result + "%";
				game.getEngine()->getDrawEngine()->drawDynamicText("speed", "SPEED: " + result, game.getEngine()->getDrawEngine()->getWindowWidth() - 150, 250);
			}

			_editor->draw();
			if (game.getEngine()->getParticleEngine()->getX() != 0) {
				int x = game.getEngine()->getParticleEngine()->getX();
				int y = game.getEngine()->getParticleEngine()->getY();
				SDL_Surface *surface = game.getEngine()->getParticleEngine()->getParticleSetSurface("hit");
				if (surface) {
					game.getEngine()->getDrawEngine()->refreshSurface(surface);
					game.getEngine()->getParticleEngine()->showParticleSet("hit");
					game.getEngine()->getDrawEngine()->drawParticle(surface, x, y);
				}
			}
		}
	}
}
