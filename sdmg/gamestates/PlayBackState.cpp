//
//
//  @ Project : SDMG
//  @ File Name : PlayBackState.cpp
//  @ Date : 18-12-2014
//  @ Author : 42IN13SAd
//
//


#include "PlayBackState.h"
#include "MainMenuState.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\physics\PhysicsEngine.h"
#include "model\Platform.h"
#include "model\MovablePlatform.h"
#include "model\Character.h"
#include "engine\input\InputEngine.h"
#include "GameOverState.h"
#include "lib\JSONParser.h"

#include "PauseState.h"

#include "engine\World.h"
#include "engine\audio\AudioEngine.h"
#include "helperclasses\HUD.h"
#include "engine\particle\ParticleInstance.h"
#include "engine\particle\ParticleEngine.h"

#include <thread>

namespace sdmg {
	namespace gamestates {
		void PlayBackState::init(GameBase &game)
		{
			_running = true;
			_game = &game;
			game.getEngine()->getPhysicsEngine()->resume();
			game.getEngine()->getAudioEngine()->play("bgm", 0);

			_multiplier = 1.0f;
			_step = 1.0f / 7.0f;
			_lastUpdate = std::chrono::high_resolution_clock::now();
			_timeStart = std::chrono::high_resolution_clock::now();
					
			if (!_particlesSet) {
				for (auto obj : game.getWorld()->getPlayers()) {
					game.getEngine()->getParticleEngine()->registerGameObject(obj);
				}

				game.getEngine()->getParticleEngine()->createParticleSet("hit", 200, 175, 175, 5, 5, 350, 350, "blood");
				game.getEngine()->getParticleEngine()->createParticleSet("fall", 200, 175, 350, 5, 22.5, 350, 550, "burst");
				_particlesSet = true;
			}
		}

		void PlayBackState::setHUDs(std::vector<helperclasses::HUD *> *huds)
		{
			_huds = huds;
		}

		void PlayBackState::cleanup(GameBase &game)
		{
			_running = false;

			while (!_recordQueue->empty()) {
				delete _recordQueue->front()->_action;
				delete _recordQueue->front();
				_recordQueue->pop();
			}
			delete _recordQueue;
			_recordQueue = nullptr;
			_threadSpawned = false;
		}

		void PlayBackState::pause(GameBase &game)
		{
			_game->getEngine()->getPhysicsEngine()->pause();
		}

		void PlayBackState::resume(GameBase &game)
		{
			_game->getEngine()->getPhysicsEngine()->resume();
			_lastUpdate = std::chrono::high_resolution_clock::now();
		}

		void PlayBackState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				
					switch (event.type) {
					case SDL_KEYDOWN:
						if (!event.key.repeat){
							switch (event.key.keysym.sym) {
							case SDLK_ESCAPE:
								GameOverState::getInstance().cleanup(game);
								_game->getStateManager()->changeState(MainMenuState::getInstance());
								break;
							}
						}

					case SDL_CONTROLLERBUTTONDOWN:
						switch (event.cbutton.button) {
						case SDL_CONTROLLER_BUTTON_START:
						case SDL_CONTROLLER_BUTTON_A:
							_game->getStateManager()->changeState(MainMenuState::getInstance());
							break;
						}
					}
			}
		}

		void PlayBackState::actionThread() {
			if (!_recordQueue->empty() && _running) {
				RecordStep *step = _recordQueue->front();
				//  long long timeRunning = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - _timeStart).count();
				long long timeRunning = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - _timeStart).count();

				if (step->_timestamp <= timeRunning)
				{
					RecordStep cStep = *step;

					//std::thread thread{ [&, cStep] {

					if (_running) {
						//  while (cStep._character->getBody()->GetWorld()->IsLocked());

						for (auto data : cStep._playerData) {

							data.character->getBody()->SetTransform(b2Vec2(data.x, data.y), cStep._character->getBody()->GetAngle());
							data.character->getBody()->SetLinearVelocity(b2Vec2(data.velocityX, data.velocityY));

							data.character->setHP(data.hp);
							data.character->setLives(data.lives);
							data.character->setPP(data.pp);
						}
						cStep._action->run(*_game);
						delete cStep._action;
					}
					/*
					} };
					thread.detach();
					*/
					delete step;
					_recordQueue->pop();
				}
			}
		}

		void PlayBackState::update(GameBase &game, GameTime &gameTime)
		{
			/*
			if (!_threadSpawned) {
				std::thread thread{ std::bind(&PlayBackState::actionThread, this) };
				thread.detach();
				_threadSpawned = true;
			}
			*/
			actionThread();
			
			if (game.getWorld()->isGameOver()) {
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
				_particlesSet = false;

				changeState(game, GameOverState::getInstance());
				return;
			}

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

			//  game.getEngine()->getInputEngine()->update(game);
			game.getEngine()->getDrawEngine()->update();
			game.getEngine()->getPhysicsEngine()->update();
		}

		void PlayBackState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			performDraw(game);
			game.getEngine()->getDrawEngine()->render();
		}

		void PlayBackState::performDraw(GameBase &game) {
			DrawEngine *de = game.getEngine()->getDrawEngine();
			de->draw("background");

			for (auto obj : game.getWorld()->getPlatforms())
				de->draw(obj);

			for (auto obj : game.getWorld()->getPlayers())
				de->draw(obj);

			for (helperclasses::HUD *hud : *_huds) {
				hud->draw(*de);
			}

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

		void PlayBackState::setPlaybackSteps(std::queue<RecordStep*> *recordQueue)
		{
			if (_recordQueue != nullptr) {
				while (!_recordQueue->empty()) {
					delete _recordQueue->front();
					_recordQueue->pop();
				}
				delete _recordQueue;
			}
			_recordQueue = recordQueue;
		}

	}
}