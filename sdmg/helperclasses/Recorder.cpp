#include "helperclasses\Recorder.h"
#include "lib\JSONParser.h"
#include "model\Character.h"
#include "engine\World.h"
#include "engine\GameBase.h"
#include "gamestates\LoadingState.h"

namespace sdmg
{
	namespace helperclasses {
		Recorder::Recorder() { }
		Recorder::~Recorder() { cleanup(); }

		void Recorder::start(GameBase &game, const std::string &level) {
			init();
			_level = new std::string(level);
			_enabled = true;

			for (auto player : game.getWorld()->getPlayers()) {
				registerCharacter(static_cast<model::Character&>(*player));
			}
		}

		void Recorder::init() {
			cleanup();

			_recordQueue = new std::queue<RecordItem*>();
			_characters = new std::map<model::Character*, int>();
			_recordStartTime = new std::chrono::high_resolution_clock::time_point(std::chrono::high_resolution_clock::now());
		}

		void Recorder::cleanup() {
			while (_recordQueue != nullptr && !_recordQueue->empty()) {
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

		void Recorder::record(std::string action, model::Character &character, bool keyDown) {
			if (_enabled) {
				//  int timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - *_recordStartTime).count();
				int timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - *_recordStartTime).count();

				auto it = _characters->find(&character);
				if (it != _characters->cend()) {
					RecordItem *item = new RecordItem(action, it->second, timestamp, keyDown);
					for (auto pair : *_characters) {						
						item->addPlayerData({ pair.second, pair.first->getHP(), pair.first->getLives(), pair.first->getPP(), pair.first->getX(), pair.first->getY(), pair.first->getBody()->GetLinearVelocity().x, pair.first->getBody()->GetLinearVelocity().y, static_cast<int>(pair.first->getDirection()) });
					}

					_recordQueue->push(item);
				}
			}
		}

		void Recorder::gameOver() {
			if (_enabled) {
				int timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - *_recordStartTime).count();

				RecordItem *item = new RecordItem("GameOver", 0, timestamp, false);
				for (auto pair : *_characters) {
					item->addPlayerData({ pair.second, pair.first->getHP(), pair.first->getLives(), pair.first->getPP(), pair.first->getX(), pair.first->getY(), pair.first->getBody()->GetLinearVelocity().x, pair.first->getBody()->GetLinearVelocity().y });
				}

				_recordQueue->push(item);
			}
		}

		void Recorder::registerCharacter(model::Character &character) {
			_characters->insert(std::make_pair(&character, _characterIndex++));
		}

		void Recorder::save(std::string path) {
			_enabled = false;

			JSON::JSONObject *recordingObj = new JSON::JSONObject(nullptr);
			JSON::JSONDocument *doc = JSON::JSONDocument::fromRoot(*recordingObj);

			JSON::JSONArray *characterArr = new JSON::JSONArray(recordingObj);
			
			std::map<int, model::Character*> characters;

			for (auto it : *_characters) {
				JSON::JSONObject *characterObj = new JSON::JSONObject(characterArr);
				characterObj->add("index", it.second);
				characterObj->add("key", it.first->getKey());

				JSON::JSONObject *positionObj = new JSON::JSONObject(characterObj);
				positionObj->add("x", it.first->getX());
				positionObj->add("y", it.first->getY());
				characterObj->add("position", *positionObj);
				characterArr->push(*characterObj);
				characters.insert({ it.second, it.first });
			}

			recordingObj->add("characters", *characterArr);
			recordingObj->add("level", *_level);

			JSON::JSONArray *stepArr = new JSON::JSONArray(recordingObj);
			
			while (!_recordQueue->empty()) {
				RecordItem *item = _recordQueue->front();

				JSON::JSONObject *stepObj = new JSON::JSONObject(stepArr);

				JSON::JSONArray *characterArr = new JSON::JSONArray(stepObj);

				for (auto data : item->getPlayerData()) {
					JSON::JSONObject *characterObj = new JSON::JSONObject(characterArr);
					
					characterObj->add("x", data->x);
					characterObj->add("y", data->y);
					characterObj->add("velocityx", data->velocityX);
					characterObj->add("velocityy", data->velocityY);
					characterObj->add("lives", data->lives);
					characterObj->add("hp", data->hp);
					characterObj->add("pp", data->pp);
					characterObj->add("character", data->character);
					characterObj->add("direction", data->direction);

					characterArr->push(*characterObj);
				}

				stepObj->add("characters", *characterArr);
				stepObj->add("action", item->getAction());
				stepObj->add("character", item->getCharacter());
				stepObj->add("timestamp", item->getTimestamp());
				stepObj->add("keyDown", item->getKeyDown());

				stepArr->push(*stepObj);

				delete item;
				_recordQueue->pop();
			}

			recordingObj->add("steps", *stepArr);
			doc->saveFile(path);
			delete doc;
		}
	}
}