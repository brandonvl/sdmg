//
//
//  @ Project : SDMG
//  @ File Name : PlayState.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "PlayState.h"
#include "LoadingSinglePlayerState.h"
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
#include "GameOverSurvivalState.h"
#include "PauseState.h"
#include "engine\World.h"
#include "engine\audio\AudioEngine.h"
#include "helperclasses\HUD.h"
#include "engine\particle\ParticleInstance.h"
#include "engine\particle\ParticleEngine.h"
#include "helperclasses\RandomGenerator.h"
#include "helperclasses\Recorder.h"
#include "engine\ai\EasyAIMachine.h"
#include <vector>

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
			_enemiesKilled = 0;
			
			for (auto obj : game.getWorld()->getPlayers()) {
				game.getEngine()->getParticleEngine()->registerGameObject(obj);
			}

			game.getEngine()->getParticleEngine()->createParticleSet("hit", 100, 175, 175, 5, 5, 350, 350, "blood");
			game.getEngine()->getParticleEngine()->createParticleSet("fall", 100, 175, 350, 5, 22.5, 350, 550, "burst");
			
			game.getRecorder().start(game,*_level);
		}

		void PlayState::setHUDs(std::vector<helperclasses::HUD *> *huds)
		{
			_huds = huds;
		}

		void PlayState::setLevel(std::string *level) {
			_level = new std::string(*level);
		}

		void PlayState::cleanup(GameBase &game)
		{
			delete _editor;
			delete _level;
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
					if (game.getEngine()->getInputEngine()->getUsedControllerName(event) == "keyboard")
					{
						game.getEngine()->getInputEngine()->handleEvent(event);

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
										setGameplaySpeed();
									}
									break;
								case SDLK_PAGEUP:
									if (!event.key.repeat){
										if (_multiplier > 0.1f)
										{
											_multiplier = _multiplier - 0.1;
											setGameplaySpeed();
										}
									}
									break;
								case SDLK_PAGEDOWN:
									if (!event.key.repeat)
									{
										if (_multiplier < 1.5f)
										{
											_multiplier = _multiplier + 0.1;
											setGameplaySpeed();
										}
									}
									break;
								}
								break;
							case SDL_QUIT:
								_multiplier = 1.0f;
								setGameplaySpeed();
								if (_game->getGameMode() == GameBase::GameMode::SinglePlayer)
								{
									LoadingSinglePlayerState::getInstance().unloadAll();
									//GameOverState::getInstance().cleanup(*_game);
								}
								break;
							}
						}
						else if (event.type == SDL_QUIT){
							_multiplier = 1.0f;
							setGameplaySpeed();
							if (_game->getGameMode() == GameBase::GameMode::SinglePlayer)
							{
								LoadingSinglePlayerState::getInstance().unloadAll();
								//GameOverState::getInstance().cleanup(*_game);
							}
							if (_huds) {
								for (auto it : *_huds) {
									delete it;
								}
								_huds->clear();
							}
							game.stop();
						}
					}
					else
					{
						game.getEngine()->getInputEngine()->handleControllers(event);
					}
				}
				else _editor->handleEvent(event);
			}
		}
		
		void PlayState::setGameplaySpeed()
		{
			_game->getEngine()->getPhysicsEngine()->setSpeed(_game->getEngine()->getPhysicsEngine()->getSpeed() * _multiplier);
			_game->getEngine()->getDrawEngine()->setSpeed(_game->getEngine()->getDrawEngine()->getSpeed() * _multiplier);
		}

		void PlayState::chooseRandomEnemy()
		{
			_enemiesKilled++;

			std::random_device dev;
			std::default_random_engine dre(dev());
			std::uniform_int_distribution<int> randomAdvertisement(0, (*_enemies).size() - 1);

			model::Character *enemy = nullptr;
			do{
				int enemyID = randomAdvertisement(dre);
				enemy = (*_enemies)[enemyID];
			} while (enemy->getName() == _game->getWorld()->getDeadList()[0]->getName());

			_game->getWorld()->clearDeadList();

			enemy->getBody()->SetActive(true);
			enemy->setLives(5);
			enemy->getAI()->resume();
			enemy->setState(MovableGameObject::State::RESPAWN);
			(*_huds)[1]->changeOwner(enemy);
			_game->getWorld()->addAlive(enemy);
		}

		void PlayState::setPlayer(model::Character *player)
		{
			_player = player;
		}

		void PlayState::setEnemies(std::vector<model::Character*> *enemies)
		{
			_enemies = enemies;
		}

		int PlayState::getEnemiesKilled()
		{
			return _enemiesKilled;
		}

		void PlayState::setEnemiesKilled(int count)
		{
			_enemiesKilled = count;
		}

		void PlayState::update(GameBase &game, GameTime &gameTime)
		{
			if (!_editor->isEnabled()) {
				if (_canDie && game.getWorld()->isGameOver()) {
					_multiplier = 1.0f;
					setGameplaySpeed();
					if (game.getGameMode() == GameBase::GameMode::SinglePlayer || game.getGameMode() == GameBase::GameMode::Versus)
					{
						game.getEngine()->getPhysicsEngine()->pause();
						if (game.getWorld()->getAliveList().size() > 0)
							game.getWorld()->getAliveList()[0]->die();

						auto players = game.getWorld()->getPlayers();
						for (auto p : players)
						{
							p->destroyAttackBody();
							p->destroyShootBody(*game.getEngine());
						}

						game.getEngine()->getParticleEngine()->unloadAll();

						changeState(game, GameOverState::getInstance());
						return;
					}
					else if (game.getGameMode() == GameBase::GameMode::Survival)
					{
						if (game.getWorld()->getDeadList()[0] == _player)
						{
							game.getEngine()->getPhysicsEngine()->pause();
							if (game.getWorld()->getAliveList().size() > 0)
								game.getWorld()->getAliveList()[0]->die();

							auto players = game.getWorld()->getPlayers();
							for (auto p : players)
							{
								p->destroyAttackBody();
								p->destroyShootBody(*game.getEngine());
							}

							game.getEngine()->getParticleEngine()->unloadAll();

							changeState(game, GameOverSurvivalState::getInstance());
							return;
						}
						else
						{
							Character *deadEnemy = static_cast<Character*>(game.getWorld()->getDeadList()[0]);
							deadEnemy->destroyAttackBody();
							deadEnemy->getAI()->pause();
							deadEnemy->getBody()->SetActive(false);
							chooseRandomEnemy();
						}
					}
				}

				if (_showFPS)
					_fps = game.getFPS() == _fps ? _fps : game.getFPS();
					
				auto curTime = std::chrono::high_resolution_clock::now();
				float diff = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - _lastUpdate).count() / 1000.0f;

				_lastUpdate = curTime;
				_accumulator += diff;

				for (auto it : game.getWorld()->getPlayers()) {
					it->update(&gameTime, &game);
				}

				while (_accumulator > _step) {
					for (auto obj : game.getWorld()->getPlayers())
						obj->addPP(1);
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

			if (game.getEngine()->getParticleEngine()->hasNextParticleInstance()) {
				std::vector<ParticleInstance*> container = game.getEngine()->getParticleEngine()->getNextParticleInstance();
				std::vector<ParticleInstance*>::iterator itr = container.begin();
				while (itr != container.end()) {
					SDL_Surface *surface = game.getEngine()->getParticleEngine()->getParticleSetSurface((*itr));
					game.getEngine()->getDrawEngine()->refreshSurface(surface);
					game.getEngine()->getParticleEngine()->showParticleSet((*itr));
					game.getEngine()->getDrawEngine()->drawParticle(surface, (*itr)->GetX(), (*itr)->GetY());
					itr++;
				}
			}
		}
	}
}
