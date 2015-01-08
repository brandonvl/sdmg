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
					_recordQueue->push(new RecordItem(action, it->second, timestamp, keyDown, character.getX(), character.getY(), character.getBody()->GetLinearVelocity().x, character.getBody()->GetLinearVelocity().y, character.getHP(), character.getLives(), character.getPP()));
				}
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

				stepObj->add("x", item->getX());
				stepObj->add("y", item->getY());
				stepObj->add("velocityx", item->getVelocityX());
				stepObj->add("velocityy", item->getVelocityY());
				stepObj->add("lives", item->getLives());
				stepObj->add("hp", item->getHP());
				stepObj->add("pp", item->getPP());
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