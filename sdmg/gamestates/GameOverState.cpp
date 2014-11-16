//
//
//  @ Project : SDMG
//  @ File Name : GameOverState.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#include "GameOverState.h"
#include "engine\GameTime.h"
#include "engine\Engine.h"
#include "engine\input\InputEngine.h"
#include "engine\drawing\DrawEngine.h"
#include "model\Character.h"
#include "helperclasses\HUD.h"
#include "MainMenuState.h"
#include "StatisticsState.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"
#include "engine\World.h"
#include "lib\JSONParser.h"
#include "engine\audio\AudioEngine.h"
#include "LoadingState.h"
#include "engine\physics\PhysicsEngine.h"
#include "PlayState.h"
#include "helperclasses\ProgressManager.h"


namespace sdmg {
	namespace gamestates {
		void GameOverState::init(GameBase &game)
		{
			_game = &game;
			std::cout << "Initing IntroState ... " << std::endl;

			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() - (187.5f * 3), 50.0f);

			// Create menu item
			helperclasses::menuitems::MenuTextItem *play = new helperclasses::menuitems::MenuTextItem("Replay", 0, 68, true);
			play->loadText(_game, "replay", "Replay", "trebucbd", 33);
			_menu->addMenuItem(play);

			helperclasses::menuitems::MenuTextItem *highscore = new helperclasses::menuitems::MenuTextItem("Statistics", 0, 68, false);
			highscore->loadText(_game, "statistics", "Statistics", "trebucbd", 33);
			_menu->addMenuItem(highscore);

			helperclasses::menuitems::MenuTextItem *quit = new helperclasses::menuitems::MenuTextItem("Main Menu", 0, 68, false);
			quit->loadText(_game, "main menu", "Main Menu", "trebucbd", 33);
			_menu->addMenuItem(quit);

			const std::vector<GameObject*> &deadList = game.getWorld()->getDeadList();
			Uint8 color = 255;
			for (int i = deadList.size() - 1; i >= 0; i--) {
				int rank = (deadList.size() - i);
				game.getEngine()->getDrawEngine()->loadText("rank" + std::to_string(rank), std::to_string(rank) + ". " + deadList[i]->getName(), { color, color, color }, "arial", 54);
				std::string asd = deadList[i]->getName();
				color = 64;
			}

			_characterCount = deadList.size();
			model::Character *chas = static_cast<model::Character*>(deadList[_characterCount - 1]);
			game.getEngine()->getDrawEngine()->load("winner", "assets/characters/" + chas->getKey() + "/win.sprite");

			// Update statistics
			JSON::JSONArray &statistics = ProgressManager::getInstance().getStatistics();
			
			for (auto rank = 0; rank < deadList.size(); rank++) {

				for (auto i = 0; i < statistics.size(); i++) {
					JSON::JSONObject &characterObj = statistics.getObject(i);

					if (characterObj.getString("name") == deadList.at(rank)->getName()) {
						if (rank == (deadList.size() - 1))
							characterObj.getVariable("wins").setValue(std::to_string(1 + characterObj.getInt("wins")));
						else
							characterObj.getVariable("losses").setValue(std::to_string(1 + characterObj.getInt("losses")));
						break;
					}
				}
			}
			
			game.getEngine()->getDrawEngine()->load("gameoverbackground", "assets/screens/gameover");


			//game.getEngine()->getAudioEngine()->load("winner", "assets/levels/background");
			game.getEngine()->getAudioEngine()->stopMusic();
			game.getEngine()->getAudioEngine()->load("winner", "assets/sounds/effects/win.ogg", AUDIOTYPE::SOUND_EFFECT);
			game.getEngine()->getAudioEngine()->play("winner", 0);
		}

		void GameOverState::menuAction(MenuItem *item)
		{
			std::string tag = item->getTag();

			if (tag == "Replay") {
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

				// changeState(*_game, LoadingState::getInstance());
			}
			else if (tag == "Statistics") {
				_replay = false;
				_game->getStateManager()->pushState(StatisticsState::getInstance());
			}
			else if (tag == "Main Menu") {
				_replay = false;
				changeState(*_game, MainMenuState::getInstance());
			}
		}

		void GameOverState::cleanup(GameBase &game)
		{
			delete _menu;
			if (_replay)
			{
				DrawEngine *de = game.getEngine()->getDrawEngine();
				de->unload("winner");
				de->unload("gameoverbackground");
				de->unloadText("replay");
				de->unloadText("statistics");
				de->unloadText("main menu");

				game.getEngine()->getAudioEngine()->unload("winner");

				for (int i = 1; i <= _characterCount; i++) {
					std::string asd = "rank" + std::to_string(i);
					de->unloadText("rank" + std::to_string(i));
				}

				PlayState::getInstance().resume(game);
			}
			else
			{
				game.getEngine()->getPhysicsEngine()->cleanUp();
				game.getEngine()->getAudioEngine()->unload("winner");
				game.getEngine()->getAudioEngine()->unload("bgm");
				game.getEngine()->getDrawEngine()->unloadAll();
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
				huds = nullptr;

				//game.getStateManager()->cleanupOthers();
			}
		}

		void GameOverState::pause(GameBase &game)
		{
			std::cout << "Pausing GameOverState ... " << std::endl;
		}

		void GameOverState::resume(GameBase &game)
		{
			std::cout << "Resuming GameOverState ... " << std::endl;
		}

		void GameOverState::handleEvents(GameBase &game, GameTime &gameTime)
		{
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
						menuAction(_menu->getSelectedMenuItem());
						break;
					}
				}
			}
		}

		void GameOverState::update(GameBase &game, GameTime &gameTime)
		{
			//std::cout << "Updating GameOverState ... " << std::endl;
		}

		void GameOverState::draw(GameBase &game, GameTime &gameTime)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("gameoverbackground");
			game.getEngine()->getDrawEngine()->draw("winner", 190, 190);

			for (int i = 1; i <= _characterCount; i++) {
				game.getEngine()->getDrawEngine()->drawText("rank" + std::to_string(i), 717, 280 + (i * 100));
			}

			_menu->draw(&game);
			game.getEngine()->getDrawEngine()->render();
		}
	}
}