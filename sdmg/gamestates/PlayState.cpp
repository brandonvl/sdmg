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
			_game = &game;
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
								if (_editMode) disableEditMode(game);
								else enableEditMode(game);
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

		void PlayState::enableEditMode(GameBase &game) {
			_editMode = true;
			_hitboxes = new std::map<GameObject*, input::Mouse::Hitbox*>();

			InputEngine *inputEngine = game.getEngine()->getInputEngine();

			for (auto obj : game.getWorld()->getGameObjects()) {
				_hitboxes->insert(std::make_pair(obj, inputEngine->getMouse().setClickAction(obj->getX() * 20.0f - obj->getWidth() / 2, obj->getY() * 20.0f - obj->getHeight() / 2, obj->getWidth(), obj->getHeight(), (std::function<void()>)[&, obj] { selectObject(*obj); })));
			}

			inputEngine->getMouse().setMouseMoveAction((std::function<void(int x, int y)>)[&](int x, int y) { PlayState::mouseMove(x, y); });
			inputEngine->getMouse().setMouseUpAction((std::function<void()>)[&] { _curSelectedObject = nullptr; });
			inputEngine->setMouseEnabled(true);
		}

		void PlayState::mouseMove(int x, int y) {
			if (_curSelectedObject) {
				_curSelectedObject->getBody()->SetTransform(b2Vec2((x - _mouseDownX) / 20.f, (y - _mouseDownY) / 20.0f), _curSelectedObject->getBody()->GetAngle());

				input::Mouse::Hitbox *hitbox = _hitboxes->at(_curSelectedObject);
				hitbox->x = _curSelectedObject->getX() * 20.0f - _curSelectedObject->getWidth() / 2;
				hitbox->y = _curSelectedObject->getY() * 20.0f - _curSelectedObject->getHeight() / 2;
			}
		}

		void PlayState::disableEditMode(GameBase &game) {
			_editMode = false;

			game.getEngine()->getInputEngine()->getMouse().clear();

			delete _hitboxes;
			_hitboxes = nullptr;
			
			game.getEngine()->getInputEngine()->setMouseEnabled(false);
		}

		void PlayState::selectObject(GameObject &gameObject) {
			_mouseDownX = _game->getEngine()->getInputEngine()->getMouse().getX() - gameObject.getX() * 20.0f;
			_mouseDownY = _game->getEngine()->getInputEngine()->getMouse().getY() - gameObject.getY() * 20.0f;
			_curSelectedObject = &gameObject;
		}

		void PlayState::update(GameBase &game, GameTime &gameTime)
		{
			if (!_editMode) {
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
			//DrawEngine *de = game.getEngine()->getDrawEngine();
			game.getEngine()->getDrawEngine()->draw("background");

			if (_showHitBoxes)
				game.getEngine()->getDrawEngine()->drawBodies(game.getEngine()->getPhysicsEngine()->getBodyList());

			if (_showClickBoxes)
				game.getEngine()->getDrawEngine()->drawHitBoxes(game.getEngine()->getInputEngine()->getMouse().getClickBoxes());

			for (auto obj : game.getWorld()->getGameObjects())
				game.getEngine()->getDrawEngine()->draw(obj);
			
			if (_huds) {
				for (helperclasses::HUD *hud : *_huds) {
					hud->draw(*game.getEngine()->getDrawEngine());
				}
			}

			if (_showFPS)
				game.getEngine()->getDrawEngine()->drawDynamicText("fps", "FPS: " + std::to_string(_fps), game.getEngine()->getDrawEngine()->getWindowWidth() - 100, 10);
		}
	}
}
