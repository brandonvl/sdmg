//
//
//  @ Project : SDMG
//  @ File Name : GameOverSurvivalState.cpp
//  @ Date : 10-12-2014
//  @ Author : 42IN13SAd
//
//

#include "GameOverSurvivalState.h"
#include "PlayState.h"
#include "helperclasses\HUD.h"
#include "engine\GameTime.h"
#include "engine\Engine.h"
#include "engine\input\InputEngine.h"
#include "engine\drawing\DrawEngine.h"
#include "model\Character.h"
#include "MainMenuState.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"
#include "engine\World.h"
#include "lib\JSONParser.h"
#include "engine\audio\AudioEngine.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\particle\ParticleEngine.h"
#include "helperclasses\ProgressManager.h"


namespace sdmg {
	namespace gamestates {
		void GameOverSurvivalState::init(GameBase &game)
		{
			_game = &game;

			_game->getEngine()->getPhysicsEngine()->pause();

			_replay = false;
			
			std::cout << "Initing IntroState ... " << std::endl;

			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() - (187.5f * 3), 50.0f, game);

			const std::vector<GameObject*> &deadList = game.getWorld()->getDeadList();

			model::Character *chas = static_cast<model::Character*>(deadList[0]);
			game.getEngine()->getDrawEngine()->load("winner", "assets/characters/" + chas->getKey() + "/win.sprite");
			
			_menu->addMenuTextItem("Replay", (std::function<void()>)std::bind(&GameOverSurvivalState::replay, this));
			_menu->addMenuTextItem("Main menu", (std::function<void()>)[&] {
				changeState(*_game, MainMenuState::getInstance());
			});

			game.getEngine()->getDrawEngine()->loadText("enemies_killed", std::to_string(PlayState::getInstance().getEnemiesKilled()) , { 255, 255, 255 }, "arial", 54);
			game.getEngine()->getDrawEngine()->load("gameoversurvivalbackground", "assets/screens/gameover");

			game.getEngine()->getAudioEngine()->stopMusic();
			game.getEngine()->getAudioEngine()->load("winner", "assets/sounds/effects/win.ogg", AUDIOTYPE::SOUND_EFFECT);
			game.getEngine()->getAudioEngine()->play("winner", 0);
		}

		// Even checken of dit wel klopt voor survival mode
		void GameOverSurvivalState::replay() {
			/*
			_game->getWorld()->resetWorld();
			const std::vector<GameObject*> &aliveList = _game->getWorld()->getAliveList();

			for (int i = 0; i < aliveList.size(); i++)
			{
				model::Character *character = static_cast<model::Character*>(aliveList[i]);
				character->revive();
				character->setState(MovableGameObject::State::RESPAWN);
			}

			_game->getEngine()->getPhysicsEngine()->resetBobs();

			_replay = true;
			_game->getEngine()->getPhysicsEngine()->resume();
			changeState(*_game, PlayState::getInstance());
			*/
		}

		void GameOverSurvivalState::cleanup(GameBase &game)
		{
			delete _menu;
			_menu = nullptr;

			if (_replay)
			{
				DrawEngine *de = game.getEngine()->getDrawEngine();
				de->unload("winner");
				de->unload("gameoversurvivalbackground");
				de->unloadText("replay");
				de->unloadText("main menu");
				game.getEngine()->getDrawEngine()->unloadText("enemies_killed");

				game.getEngine()->getAudioEngine()->unload("winner");
				
				PlayState::getInstance().resume(game);
				_replay = false;
			}
			else
			{
				game.getEngine()->getPhysicsEngine()->cleanUp();
				game.getEngine()->getAudioEngine()->unloadAll();
				game.getEngine()->getDrawEngine()->unloadAll();
				//  game.getEngine()->getParticleEngine()->unloadAll();
				game.getEngine()->getInputEngine()->clearBindings();

				game.getWorld()->clearWorld();

				std::vector<HUD*> *huds = PlayState::getInstance()._huds;

				if (huds) {
					for (auto it : *huds) {
						delete it;
					}
					huds->clear();
				}
				delete huds;
			}

			game.getEngine()->getInputEngine()->getMouse().clear();
		}

		void GameOverSurvivalState::handleEvents(GameBase &game, GameTime &gameTime)
		{
			SDL_Event event;

			if (SDL_PollEvent(&event))
			{
				game.getEngine()->getInputEngine()->getMouse().handleMouseEvent(event);

				if (event.type == SDL_QUIT)
				{
					game.stop();
				}

				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						changeState(game, MainMenuState::getInstance());
						break;
					case SDLK_DOWN:
						_menu->selectNext();
						break;
					case SDLK_UP:
						_menu->selectPrevious();
						break;
					case SDLK_KP_ENTER:
					case SDLK_RETURN:
						_menu->doAction();
						break;
					}
				}
			}
		}

		void GameOverSurvivalState::update(GameBase &game, GameTime &gameTime)
		{
			//std::cout << "Updating GameOverState ... " << std::endl;
		}

		void GameOverSurvivalState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("gameoversurvivalbackground");
			game.getEngine()->getDrawEngine()->draw("winner", 190, 190);

			game.getEngine()->getDrawEngine()->drawText("enemies_killed", 900, 325);
			
			_menu->draw(&game);
			game.getEngine()->getDrawEngine()->render();
		}
	}
}