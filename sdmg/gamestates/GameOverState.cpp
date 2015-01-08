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
#include "engine\ai\AIMachine.h"
#include "helperclasses\HUD.h"
#include "MainMenuState.h"
#include "StatisticsState.h"
#include "UnlockedState.h"
#include "helperclasses\Menu.h"
#include "helperclasses\menuitems\MenuTextItem.h"
#include "engine\World.h"
#include "lib\JSONParser.h"
#include "engine\audio\AudioEngine.h"
#include "LoadingSinglePlayerState.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\particle\ParticleEngine.h"
#include "PlayState.h"
#include "PlayBackState.h"
#include "helperclasses\ProgressManager.h"
#include "helperclasses\Recorder.h"


namespace sdmg {
	namespace gamestates {
		void GameOverState::init(GameBase &game)
		{
			_game = &game;

			_game->getEngine()->getPhysicsEngine()->pause();

			_savedReplay = false;

			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() - (187.5f * 3), 50.0f, game);

			const std::vector<GameObject*> &deadList = game.getWorld()->getDeadList();
			bool unlocked = false;

			if (game.getGameMode() == GameBase::GameMode::SinglePlayer)
			{
				static_cast<Character*>(game.getWorld()->getPlayers()[1])->getAI()->pause();

				if (!LoadingSinglePlayerState::getInstance().hasFinished())
				{
					if (static_cast<Character*>(deadList[1])->getKey() == LoadingSinglePlayerState::getInstance().getPlayerName())
						_menu->addMenuTextItem("Next", (std::function<void()>)std::bind(&GameOverState::next, this));
				}
				else if (static_cast<Character*>(deadList[1])->getKey() == LoadingSinglePlayerState::getInstance().getPlayerName())
				{
					ProgressManager &manager = ProgressManager::getInstance();

					// Deze gebruiken als Esté de keys heeft toegevoegd in de config
					//if (!manager.isUnlockedCharacter(LoadingSinglePlayerState::getInstance().getPlayerName()))
					if (!manager.isUnlockedCharacter(static_cast<Character*>(deadList[0])->getName()))
					{
						UnlockedState::getInstance().setPlayerName(static_cast<Character*>(deadList[0])->getKey());
						UnlockedState::getInstance().setLevelName(LoadingSinglePlayerState::getInstance().getLevelName());
						manager.setIsUnlockedCharacter(static_cast<Character*>(deadList[0])->getName(), true);
						unlocked = true;
					}

					// Deze gebruiken als Esté de keys heeft toegevoegd in de config
					if (!manager.isUnlockedLevel(LoadingSinglePlayerState::getInstance().getLevelName()))
					{
						UnlockedState::getInstance().setPlayerName(static_cast<Character*>(deadList[0])->getKey());
						UnlockedState::getInstance().setLevelName(LoadingSinglePlayerState::getInstance().getLevelName());
						manager.setIsUnlockedLevel(LoadingSinglePlayerState::getInstance().getLevelName(), true);
						unlocked = true;
					}
					
				}
				_menu->addMenuTextItem("Statistics", (std::function<void()>)[&] { _game->getStateManager()->pushState(StatisticsState::getInstance()); });
			}
			else if (game.getGameMode() == GameBase::GameMode::Versus)
			{
				_menu->addMenuTextItem("Replay", (std::function<void()>)std::bind(&GameOverState::replay, this));
				_menu->addMenuTextItem("Save replay", (std::function<void()>)std::bind(&GameOverState::saveReplay, this));
				_menu->addMenuTextItem("Statistics", (std::function<void()>)[&] { _game->getStateManager()->pushState(StatisticsState::getInstance()); });
			}

			_menu->addMenuTextItem("Main menu", (std::function<void()>)[&] {
				if (game.getGameMode() == GameBase::GameMode::SinglePlayer)
					LoadingSinglePlayerState::getInstance().unloadAll();
				changeState(*_game, MainMenuState::getInstance());
			});
		
			Uint8 color = 255;
			for (int i = deadList.size() - 1; i >= 0; i--) {
				int rank = (deadList.size() - i);
				game.getEngine()->getDrawEngine()->loadText("rank" + std::to_string(rank), std::to_string(rank) + ". " + deadList[i]->getName(), { color, color, color }, "arial", 54);
				std::string asd = deadList[i]->getName();
				color = 64;
			}

			_characterCount = deadList.size();
			model::Character *chas = static_cast<model::Character*>(deadList[_characterCount - 1]);
			game.getEngine()->getDrawEngine()->load("winner", "assets/characters/" + chas->getKey() + "/win");

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
			
			// Save progress if autosave is enabled
			if (ProgressManager::getInstance().autosaveEnabled())
				ProgressManager::getInstance().save();
			
			game.getEngine()->getDrawEngine()->load("gameoverbackground", "assets/screens/gameover");


			//game.getEngine()->getAudioEngine()->load("winner", "assets/levels/background");
			game.getEngine()->getAudioEngine()->stopMusic();
			game.getEngine()->getAudioEngine()->load("winner", "assets/sounds/effects/win.ogg", AUDIOTYPE::SOUND_EFFECT);
			game.getEngine()->getAudioEngine()->play("winner", 0);

			if (unlocked)
			{
				unlocked = false;
				game.getStateManager()->pushState(UnlockedState::getInstance());
			}
		}

		void GameOverState::saveReplay() {

			if (!_savedReplay) {
				_game->getRecorder().save("assets/playbacks/recording");

				_savedReplay = true;

				static_cast<helperclasses::menuitems::MenuTextItem*>(_menu->getSelectedMenuItem())->setText("Saved", *_game);
			}
		}

		void GameOverState::replay() {
			_game->getWorld()->resetWorld();
			const std::vector<GameObject*> &aliveList = _game->getWorld()->getAliveList();

			for (int i = 0; i < aliveList.size(); i++)
			{
				model::Character *character = static_cast<model::Character*>(aliveList[i]);
				character->revive();
				//  character->setState(MovableGameObject::State::RESPAWN);
			}

			_game->getEngine()->getPhysicsEngine()->resetBobs();

			_replay = true;
			_game->getEngine()->getPhysicsEngine()->resume();
			changeState(*_game, PlayState::getInstance());
		}

		void GameOverState::next() {
			_replay = false;
			changeState(*_game, LoadingSinglePlayerState::getInstance());
			LoadingSinglePlayerState::getInstance().loadNextFight();
		}

		void GameOverState::cleanup(GameBase &game)
		{
			delete _menu;
			_menu = nullptr;

			game.getRecorder().reset();

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
				_replay = false;
			}
			else
			{
				delete PlayState::getInstance()._level;
				PlayState::getInstance()._level = nullptr;

				game.getWorld()->destroyShootBodies();
				game.getEngine()->getAudioEngine()->unloadAll();
				game.getEngine()->getDrawEngine()->unloadAll();
				game.getEngine()->getPhysicsEngine()->cleanUp();
				game.getEngine()->getParticleEngine()->unloadAll();
				game.getEngine()->getInputEngine()->clearBindings();

				game.getWorld()->clearWorld();

				std::vector<HUD*> *huds = game.getGameMode() == GameBase::GameMode::Playback ? PlayBackState::getInstance()._huds : PlayState::getInstance()._huds;

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

		void GameOverState::handleEvents(GameBase &game, GameTime &gameTime)
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