#include "helperclasses\Recorder.h"
#include "lib\JSONParser.h"
#include "model\Character.h"
#include "engine\World.h"
#include "engine\GameBase.h"

namespace sdmg
{
	namespace helperclasses {
		Recorder::Recorder() { }
		Recorder::~Recorder() { cleanup(); }

		void Recorder::start(GameBase &game) {
			init();	
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
			delete _characters;
			delete _recordStartTime;
		}

		void Recorder::record(std::string action, model::Character &character, bool keyDown) {
			if (_enabled) {
				int timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - *_recordStartTime).count();

				auto it = _characters->find(&character);
				if (it != _characters->cend())
					_recordQueue->push(new RecordItem(action, it->second, timestamp, keyDown));
			}
		}

		void Recorder::registerCharacter(model::Character &character) {
			_characters->insert(std::make_pair(&character, ++_characterIndex));
		}

		void Recorder::save(std::string path) {
			_enabled = false;

			JSON::JSONObject *recordingObj = new JSON::JSONObject(nullptr);
			JSON::JSONDocument *doc = JSON::JSONDocument::fromRoot(*recordingObj);

			JSON::JSONArray *characterArr = new JSON::JSONArray(recordingObj);

			for (auto it : *_characters) {
				JSON::JSONObject *characterObj = new JSON::JSONObject(characterArr);
				characterObj->add("index", it.second);
				characterObj->add("key", it.first->getKey());

				JSON::JSONObject *positionObj = new JSON::JSONObject(characterObj);
				positionObj->add("x", it.first->getX());
				positionObj->add("y", it.first->getY());
				characterObj->add("position", *positionObj);

				characterArr->push(*characterObj);
			}

			recordingObj->add("characters", *characterArr);

			JSON::JSONArray *stepArr = new JSON::JSONArray(recordingObj);

			while (!_recordQueue->empty()) {
				RecordItem *item = _recordQueue->front();

				JSON::JSONObject *stepObj = new JSON::JSONObject(stepArr);

				stepObj->add("action", item->getAction());
				stepObj->add("character", item->getCharacter());
				stepObj->add("timestamp", item->getTimestamp());
				stepObj->add("keyDown", item->getKeyDown());

				stepArr->push(*stepObj);
				_recordQueue->pop();
			}

			recordingObj->add("steps", *stepArr);
			doc->saveFile(path);
		}
	}
}