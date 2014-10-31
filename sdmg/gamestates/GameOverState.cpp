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
#include "MainMenuState.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"
#include "engine\World.h"
#include "helperclasses\statistics\Statistics.h"


#include "LoadingState.h"
#include "engine\physics\PhysicsEngine.h"
#include "PlayState.h"


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

			game.getEngine()->getDrawEngine()->load("background", "assets/screens/gameover");

			std::vector<std::vector<std::string>> statistics = Statistics::getInstance().load();
			for (auto rank = 0; rank < deadList.size(); rank++) {
				// Get character name
				std::string charname = "";
				for (auto c : deadList.at(rank)->getName()) {
					if (c != ' ')
						charname += tolower(c);
					else break;
				}

				for (auto i = 0; i < statistics.size(); i++) {
					if (statistics.at(i).at(0) == charname) {
						if (rank == (deadList.size() - 1))
							statistics.at(i).at(1) = std::to_string(1 + std::stoi(statistics.at(i).at(1)));
						else
							statistics.at(i).at(2) = std::to_string(1 + std::stoi(statistics.at(i).at(2)));
						break;
					}

				}
			}

			Statistics::getInstance().save(statistics);

			game.getEngine()->getDrawEngine()->load("gameoverbackground", "assets/screens/gameover");

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

				_replay = true;
				_game->getEngine()->getPhysicsEngine()->resume();
				//changeState(*_game, PlayState::getInstance());
				_game->getStateManager()->popState();


				// changeState(*_game, LoadingState::getInstance());
				/*
				_replay = true;

				*/
			}
			else if (tag == "Main Menu") {
				_replay = false;
				changeState(*_game, MainMenuState::getInstance());
			}
		}

		void GameOverState::cleanup(GameBase &game)
		{
			if (_replay)
			{
				DrawEngine *de = game.getEngine()->getDrawEngine();
				de->unload("winner");
				de->unload("gameoverbackground");
				de->unloadText("replay");
				de->unloadText("main menu");
				delete _menu;

				for (int i = 1; i <= _characterCount; i++) {
					std::string asd = "rank" + std::to_string(i);
					de->unloadText("rank" + std::to_string(i));
				}
			}
			else
			{
				game.getEngine()->getDrawEngine()->unloadAll();
				game.getEngine()->getInputEngine()->clearBindings();

				game.getWorld()->clearWorld();

				game.getStateManager()->cleanupOthers();
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
					case SDLK_1:
						std::cout << "Key 1 pressed. Switching State.. " << std::endl;
						//changeState(game, LoadingState::getInstance());
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