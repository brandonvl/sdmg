#include "helperclasses\Recorder.h"
#include "model\Character.h"
#include "engine\World.h"
#include "engine\GameBase.h"
#include "gamestates\LoadingState.h"
#include "engine/util/FileManager.h"

namespace sdmg
{
	namespace helperclasses
	{
		Recorder::Recorder()
		{
		}
		Recorder::~Recorder()
		{
			cleanup();
		}

		void Recorder::start(GameBase& game, const std::string& level)
		{
			init();
			_level = new std::string(level);
			_enabled = true;

			for (auto player : game.getWorld()->getPlayers())
			{
				registerCharacter(static_cast<model::Character&>(*player));
			}
		}

		void Recorder::init()
		{
			cleanup();

			_recordQueue = new std::queue<RecordItem*>();
			_characters = new std::map<model::Character*, int>();
			_recordStartTime = new std::chrono::high_resolution_clock::time_point(std::chrono::high_resolution_clock::now());
		}

		void Recorder::cleanup()
		{
			while (_recordQueue != nullptr && !_recordQueue->empty())
			{
				delete _recordQueue->front();
				_recordQueue->pop();
			}

			delete _recordQueue;
			_recordQueue = nullptr;
			delete _characters;
			_characters = nullptr;
			delete _recordStartTime;
			_recordStartTime = nullptr;
			delete _level;
			_level = nullptr;

			_characterIndex = 0;
		}

		void Recorder::record(std::string action, model::Character& character, bool keyDown)
		{
			if (_enabled)
			{
				//  int timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - *_recordStartTime).count();
				int timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - *_recordStartTime).count();

				auto it = _characters->find(&character);
				if (it != _characters->cend())
				{
					RecordItem* item = new RecordItem(action, it->second, timestamp, keyDown);
					for (auto pair : *_characters)
					{
						item->addPlayerData({ pair.second, pair.first->getHP(), pair.first->getLives(), pair.first->getPP(), pair.first->getX(), pair.first->getY(), pair.first->getBody()->GetLinearVelocity().x, pair.first->getBody()->GetLinearVelocity().y, static_cast<int>(pair.first->getDirection()) });
					}

					_recordQueue->push(item);
				}
			}
		}

		void Recorder::gameOver()
		{
			if (_enabled)
			{
				int timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - *_recordStartTime).count();

				RecordItem* item = new RecordItem("GameOver", 0, timestamp, false);
				for (auto pair : *_characters)
				{
					item->addPlayerData({ pair.second, pair.first->getHP(), pair.first->getLives(), pair.first->getPP(), pair.first->getX(), pair.first->getY(), pair.first->getBody()->GetLinearVelocity().x, pair.first->getBody()->GetLinearVelocity().y });
				}

				_recordQueue->push(item);
			}
		}

		void Recorder::registerCharacter(model::Character& character)
		{
			_characters->insert(std::make_pair(&character, _characterIndex++));
		}

		void Recorder::save(std::string path)
		{
			_enabled = false;

			nlohmann::json jsonPlayBack;

			for (auto& characterIterator : *_characters)
			{
				auto& character = characterIterator.first;
				nlohmann::json characterObject;
				characterObject["index"] = characterIterator.second;
				characterObject["key"] = character->getKey();

				characterObject["position"]["x"] = character->getX();
				characterObject["position"]["y"] = character->getY();

				jsonPlayBack["characters"].push_back(characterObject);
			}

			jsonPlayBack["level"] = *_level;

			while (!_recordQueue->empty())
			{
				auto recordItem = _recordQueue->front();

				nlohmann::json stepObject;

				auto& playerData = recordItem->getPlayerData();
				for (int i = 0; i < playerData.size(); i++)
				{
					stepObject["characters"][i]["x"] = playerData[i]->x;
					stepObject["characters"][i]["y"] = playerData[i]->y;
					stepObject["characters"][i]["velocityx"] = playerData[i]->velocityX;
					stepObject["characters"][i]["velocityy"] = playerData[i]->velocityY;
					stepObject["characters"][i]["lives"] = playerData[i]->lives;
					stepObject["characters"][i]["hp"] = playerData[i]->hp;
					stepObject["characters"][i]["pp"] = playerData[i]->pp;
					stepObject["characters"][i]["character"] = playerData[i]->character;
					stepObject["characters"][i]["direction"] = playerData[i]->direction;
				}

				stepObject["action"] = recordItem->getAction();
				stepObject["character"] = recordItem->getCharacter();
				stepObject["timestamp"] = recordItem->getTimestamp();
				stepObject["keyDown"] = recordItem->getKeyDown();

				jsonPlayBack["steps"].push_back(stepObject);

				delete recordItem;
				_recordQueue->pop();
			}

			engine::util::FileManager::getInstance().saveJsonContentToFile(path, jsonPlayBack);
		}
	}
}