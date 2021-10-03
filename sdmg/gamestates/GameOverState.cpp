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
#include "engine\audio\AudioEngine.h"
#include "LoadingSinglePlayerState.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\particle\ParticleEngine.h"
#include "PlayState.h"
#include "PlayBackState.h"
#include "helperclasses\ProgressManager.h"
#include "helperclasses\Recorder.h"
#include "engine\GameTime.h"
#include <json.hpp>


namespace sdmg
{
	namespace gamestates
	{
		void GameOverState::init(GameBase& game)
		{
			_game = &game;

			_game->getEngine()->getPhysicsEngine()->pause();

			_isLoaded = false;
			_savedReplay = false;
			_enteredGameOverState = game.getGameTime()->getTotalSecondsRunning();

			game.getEngine()->getAudioEngine()->stopMusic();

			_menu = new Menu(game.getEngine()->getDrawEngine()->getWindowWidth() - (187.5f * 3), 50.0f, game);

			const std::vector<GameObject*>& deadList = game.getWorld()->getDeadList();
			bool unlocked = false;

			model::Character* chas = nullptr;

			if (game.getGameMode() == GameBase::GameMode::SinglePlayer)
			{
				static_cast<Character*>(game.getWorld()->getPlayers()[1])->getAI()->pause();

				if (!LoadingSinglePlayerState::getInstance().hasFinished())
				{
					if (static_cast<Character*>(deadList[1])->getKey() == LoadingSinglePlayerState::getInstance().getPlayerName())
					{
						game.getEngine()->getDrawEngine()->load("gameoverbackground", "assets/screens/winner");
						_menu->addMenuTextItem("Next", (std::function<void()>)std::bind(&GameOverState::next, this));
						game.getEngine()->getAudioEngine()->load("winner", "assets/sounds/effects/win.ogg", AUDIOTYPE::SOUND_EFFECT);
						game.getEngine()->getAudioEngine()->play("winner", 0);
					}
					else
					{
						chas = static_cast<model::Character*>(deadList[0]);
						game.getEngine()->getDrawEngine()->load("gameoverbackground", "assets/screens/loser");

						game.getEngine()->getAudioEngine()->load("loser", "assets/sounds/effects/lose.ogg", AUDIOTYPE::SOUND_EFFECT);
						game.getEngine()->getAudioEngine()->play("loser", 0);
					}
				}
				else if (static_cast<Character*>(deadList[1])->getKey() == LoadingSinglePlayerState::getInstance().getPlayerName())
				{
					game.getEngine()->getDrawEngine()->load("gameoverbackground", "assets/screens/winner");

					ProgressManager& manager = ProgressManager::getInstance();

					if (!manager.isUnlockedCharacter(static_cast<Character*>(deadList[0])->getName()))
					{
						std::string asd = static_cast<Character*>(deadList[0])->getKey();
						std::string has = LoadingSinglePlayerState::getInstance().getLevelName();
						UnlockedState::getInstance().setPlayerName(static_cast<Character*>(deadList[0])->getKey());
						UnlockedState::getInstance().setLevelName(LoadingSinglePlayerState::getInstance().getLevelName());
						manager.setIsUnlockedCharacter(static_cast<Character*>(deadList[0])->getName(), true);
						manager.setIsUnlockedLevel(LoadingSinglePlayerState::getInstance().getLevelName(), true);
						//  UnlockedState::getInstance().cleanup(game);
						unlocked = true;
					}

					game.getEngine()->getAudioEngine()->load("winner", "assets/sounds/effects/win.ogg", AUDIOTYPE::SOUND_EFFECT);
					game.getEngine()->getAudioEngine()->play("winner", 0);
				}
				_menu->addMenuTextItem("Achievements", (std::function<void()>)[&]
				{
					_game->getStateManager()->pushState(StatisticsState::getInstance());
				});
			}
			else if (game.getGameMode() == GameBase::GameMode::Versus)
			{
				game.getEngine()->getAudioEngine()->load("winner", "assets/sounds/effects/win.ogg", AUDIOTYPE::SOUND_EFFECT);
				game.getEngine()->getAudioEngine()->play("winner", 0);
				game.getEngine()->getDrawEngine()->load("gameoverbackground", "assets/screens/winner");
				_menu->addMenuTextItem("Play again", (std::function<void()>)std::bind(&GameOverState::replay, this));
				_menu->addMenuTextItem("Save replay", (std::function<void()>)std::bind(&GameOverState::saveReplay, this));
				_menu->addMenuTextItem("Statistics", (std::function<void()>)[&]
				{
					_game->getStateManager()->pushState(StatisticsState::getInstance());
				});
			}
			else if (game.getGameMode() == GameBase::GameMode::Playback)
			{
				game.getEngine()->getAudioEngine()->load("winner", "assets/sounds/effects/win.ogg", AUDIOTYPE::SOUND_EFFECT);
				game.getEngine()->getAudioEngine()->play("winner", 0);
				game.getEngine()->getDrawEngine()->load("gameoverbackground", "assets/screens/winner");
			}

			_menu->addMenuTextItem("Main menu", (std::function<void()>)[&]
			{
				if (game.getGameMode() == GameBase::GameMode::SinglePlayer)
					LoadingSinglePlayerState::getInstance().unloadAll();
				changeState(*_game, MainMenuState::getInstance());
			});

			Uint8 color = 255;
			for (int i = deadList.size() - 1; i >= 0; i--)
			{
				int rank = (deadList.size() - i);
				game.getEngine()->getDrawEngine()->loadText("rank" + std::to_string(rank), std::to_string(rank) + ". " + deadList[i]->getName(), { color, color, color }, "arial", 54);
				std::string asd = deadList[i]->getName();
				color = 64;
			}

			_characterCount = deadList.size();
			if (!chas)
				chas = static_cast<model::Character*>(deadList[_characterCount - 1]);

			game.getEngine()->getDrawEngine()->load("winner", "assets/characters/" + chas->getKey() + "/win");

			// Update statistics
			auto& jsonCharacterStatistics = ProgressManager::getInstance().getStatistics();

			auto findCharacterIndex = [](const std::string& characterName, nlohmann::json& jsonArray)
			{
				for (int i = 0; i < jsonArray.size(); i++)
				{
					if (jsonArray[i]["name"].get<std::string>() == characterName)
						return i;
				}

				return -1;
			};

			for (int i = 0; i < deadList.size(); i++)
			{
				auto characterIndex = findCharacterIndex(deadList[i]->getName(), jsonCharacterStatistics);
				if (i == (deadList.size() - 1)) // very safe way to determine winner
				{
					auto incrementedWins = jsonCharacterStatistics[characterIndex]["wins"].get<int>() + 1;
					jsonCharacterStatistics[characterIndex]["wins"] = incrementedWins;
				}
				else
				{
					auto incrementedLosses = jsonCharacterStatistics[characterIndex]["losses"].get<int>() + 1;
					jsonCharacterStatistics[characterIndex]["losses"] = incrementedLosses;
				}
			}

			// Save progress if autosave is enabled
			if (ProgressManager::getInstance().autosaveEnabled())
				ProgressManager::getInstance().save();

			if (unlocked)
			{
				if (ProgressManager::getInstance().autosaveEnabled())
					ProgressManager::getInstance().save();
				unlocked = false;
				game.getStateManager()->pushState(UnlockedState::getInstance());
			}
		}

		void GameOverState::saveReplay()
		{

			if (!_savedReplay)
			{
				static_cast<helperclasses::menuitems::MenuTextItem*>(_menu->getSelectedMenuItem())->setText("Saving...", *_game);
				redraw(*_game);

				_game->getRecorder().save("assets/playbacks/recording");

				_savedReplay = true;

				static_cast<helperclasses::menuitems::MenuTextItem*>(_menu->getSelectedMenuItem())->setText("Saved", *_game);
			}
		}

		void GameOverState::replay()
		{
			_game->getWorld()->resetWorld();
			_game->getEngine()->getInputEngine()->enableAllDevices();
			const std::vector<GameObject*>& aliveList = _game->getWorld()->getAliveList();

			for (size_t i = 0, ilen = aliveList.size(); i < ilen; i++)
			{
				model::Character* character = static_cast<model::Character*>(aliveList[i]);
				character->revive();
				//  character->setState(MovableGameObject::State::RESPAWN);
			}

			_game->getEngine()->getPhysicsEngine()->resetBobs();

			_replay = true;
			_game->getEngine()->getPhysicsEngine()->resume();
			changeState(*_game, PlayState::getInstance());
		}

		void GameOverState::next()
		{
			_replay = false;
			changeState(*_game, LoadingSinglePlayerState::getInstance());
			LoadingSinglePlayerState::getInstance().loadNextFight();
		}

		void GameOverState::cleanup(GameBase& game)
		{
			delete _menu;
			_menu = nullptr;

			game.getRecorder().reset();

			if (_replay)
			{
				DrawEngine* de = game.getEngine()->getDrawEngine();
				de->unload("winner");
				de->unload("gameoverbackground");
				de->unloadText("replay");
				de->unloadText("statistics");
				de->unloadText("main menu");

				game.getEngine()->getAudioEngine()->unload("winner");

				for (int i = 1; i <= _characterCount; i++)
				{
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

				std::vector<HUD*>* huds = game.getGameMode() == GameBase::GameMode::Playback ? PlayBackState::getInstance()._huds : PlayState::getInstance()._huds;

				if (huds)
				{
					for (auto it : *huds)
					{
						delete it;
					}
					huds->clear();
				}

				delete huds;
				huds = nullptr;


				if (game.getGameMode() != GameBase::GameMode::Edit)
				{
					delete PlayState::getInstance()._slotKeyInput;
					PlayState::getInstance()._slotKeyInput = nullptr;
					delete PlayState::getInstance()._keys;
					PlayState::getInstance()._keys = nullptr;
				}
			}

			game.getEngine()->getInputEngine()->getMouse().clear();
		}

		void GameOverState::handleEvents(GameBase& game, GameTime& gameTime)
		{
			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				game.getEngine()->getInputEngine()->getMouse().handleMouseEvent(event);

				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_DOWN:
							if (_isLoaded)
								_menu->selectNext();
							break;
						case SDLK_UP:
							if (_isLoaded)
								_menu->selectPrevious();
							break;
						case SDLK_KP_ENTER:
						case SDLK_RETURN:
							if (_isLoaded)
								_menu->doAction();
							break;
					}
				}
				else if (event.type == SDL_CONTROLLERBUTTONDOWN)
				{
					switch (event.cbutton.button)
					{
						case SDL_CONTROLLER_BUTTON_A:
							if (_isLoaded)
								_menu->doAction();
							break;
						case SDL_CONTROLLER_BUTTON_DPAD_UP:
							if (_isLoaded)
								_menu->selectPrevious();
							break;
						case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
							if (_isLoaded)
								_menu->selectNext();
							break;
					}
				}
			}
		}

		void GameOverState::update(GameBase& game, GameTime& gameTime)
		{
			if (game.getGameTime()->getTotalSecondsRunning() - _enteredGameOverState > 3)
				_isLoaded = true;
		}

		void GameOverState::draw(GameBase& game, GameTime& gameTime)
		{
			redraw(game);
		}

		void GameOverState::redraw(GameBase& game)
		{
			game.getEngine()->getDrawEngine()->prepareForDraw();
			game.getEngine()->getDrawEngine()->draw("gameoverbackground");
			game.getEngine()->getDrawEngine()->draw("winner", 190, 190);

			for (int i = 1; i <= _characterCount; i++)
			{
				game.getEngine()->getDrawEngine()->drawText("rank" + std::to_string(i), 717, 280 + (i * 75));
			}

			if (_isLoaded)
				_menu->draw(&game);

			game.getEngine()->getDrawEngine()->render();
		}
	}
}