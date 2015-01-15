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
#include "HighScoreState.h"
#include "HighScoreInputState.h"
#include "engine\ai\EasyAIMachine.h"

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

			model::Character *enemy = static_cast<model::Character*>(deadList[1]);
			enemy->getAI()->pause();

			model::Character *chas = static_cast<model::Character*>(deadList[0]);
			game.getEngine()->getDrawEngine()->load("winner", "assets/characters/" + chas->getKey() + "/win");
			
			_menu->addMenuTextItem("Replay", (std::function<void()>)std::bind(&GameOverSurvivalState::replay, this));
			_menu->addMenuTextItem("Highscores", (std::function<void()>)[&] {
				game.getStateManager()->pushState(HighScoreState::getInstance());
			});
			_menu->addMenuTextItem("Main menu", (std::function<void()>)[&] {
				changeState(*_game, MainMenuState::getInstance());
			});

			int highscore = PlayState::getInstance().getEnemiesKilled();
			game.getEngine()->getDrawEngine()->loadText("killed", "You have defeated", { 255, 255, 255 }, "arial", 54);
			game.getEngine()->getDrawEngine()->loadText("enemies_killed", std::to_string(highscore), { 255, 255, 255 }, "arial", 74);

			if (highscore == 1)
				game.getEngine()->getDrawEngine()->loadText("enemies", "enemy", { 255, 255, 255 }, "arial", 54);
			else
				game.getEngine()->getDrawEngine()->loadText("enemies", "enemies", { 255, 255, 255 }, "arial", 54);

			if (highscore < 1)
				game.getEngine()->getDrawEngine()->load("gameoversurvivalbackground", "assets/screens/loser");
			else
				game.getEngine()->getDrawEngine()->load("gameoversurvivalbackground", "assets/screens/winner");

			game.getEngine()->getAudioEngine()->stopMusic();
			game.getEngine()->getAudioEngine()->load("winner", "assets/sounds/effects/win.ogg", AUDIOTYPE::SOUND_EFFECT);
			game.getEngine()->getAudioEngine()->play("winner", 0);

			
			if (ProgressManager::getInstance().getLowestHighscore() < highscore) {
				HighScoreInputState::getInstance().setHighscore(highscore);
				_game->getStateManager()->pushState(HighScoreInputState::getInstance());
			}
			
		}

		// Even checken of dit wel klopt voor survival mode
		void GameOverSurvivalState::replay() {
			_game->getWorld()->resetWorld();
			_game->getEngine()->getInputEngine()->enableAllDevices();
			const std::vector<GameObject*> &aliveList = _game->getWorld()->getAliveList();

			for (size_t i = 0, ilen = aliveList.size(); i < ilen; i++)
			{
				model::Character *character = static_cast<model::Character*>(aliveList[i]);
				character->revive();
				character->setState(MovableGameObject::State::RESPAWN);

				if (i == 1)
					character->getAI()->resume();
			}

			_replay = true;
			_game->getEngine()->getPhysicsEngine()->resume();
			changeState(*_game, PlayState::getInstance());
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
				de->unloadText("killed");
				de->unloadText("enemies_killed");
				de->unloadText("enemies");

				game.getEngine()->getAudioEngine()->unload("winner");
				
				PlayState::getInstance().setEnemiesKilled(0);
				PlayState::getInstance().resume(game);
				_replay = false;
			}
			else
			{
				game.getWorld()->destroyShootBodies();
				game.getEngine()->getPhysicsEngine()->cleanUp();
				game.getEngine()->getAudioEngine()->unloadAll();
				game.getEngine()->getDrawEngine()->unloadAll();
				//  game.getEngine()->getParticleEngine()->unloadAll();
				game.getEngine()->getInputEngine()->clearBindings();

				game.getWorld()->clearWorld();

				delete PlayState::getInstance()._enemies;

				std::vector<HUD*> *huds = PlayState::getInstance()._huds;

				if (huds) {
					for (auto it : *huds) {
						delete it;
					}
					huds->clear();
				}
				delete huds;

				delete PlayState::getInstance()._slotKeyInput;
				PlayState::getInstance()._slotKeyInput = nullptr;
				delete PlayState::getInstance()._keys;
				PlayState::getInstance()._keys = nullptr;
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
				else if (event.type == SDL_CONTROLLERBUTTONDOWN)
				{
					switch (event.cbutton.button)
					{
					case SDL_CONTROLLER_BUTTON_B:
						changeState(game, MainMenuState::getInstance());
						break;
					case SDL_CONTROLLER_BUTTON_START:
						_menu->doAction();
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_UP:
						_menu->selectPrevious();
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
						_menu->selectNext();
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
			DrawEngine *de = game.getEngine()->getDrawEngine();

			de->prepareForDraw();
			de->draw("gameoversurvivalbackground");
			de->draw("winner", 190, 190);

			de->drawText("killed", 900 - de->getTextSize("killed")[0] / 2, 300);
			de->drawText("enemies_killed", 900 - de->getTextSize("enemies_killed")[0] / 2, 300 + de->getTextSize("killed")[1] + 25);
			de->drawText("enemies", 900 - de->getTextSize("enemies")[0] / 2, 300 + de->getTextSize("killed")[1] + de->getTextSize("enemies_killed")[1] + 50);
			
			_menu->draw(&game);
			de->render();
		}
	}
}