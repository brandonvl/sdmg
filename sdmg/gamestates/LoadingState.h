
//
//
//  @ Project : SDMG
//  @ File Name : LoadingState.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "engine\GameState.h"
#include <string>
#include <map>
#include <json.hpp>
struct SDL_Thread;

using namespace sdmg::engine;

namespace sdmg {
	namespace model {
		class Platform;
		class Character;
		class MovablePlatform;
	}
	namespace helperclasses{ class HUD; }
	namespace engine { class MovableGameObject; }
	namespace gamestates {
		class LoadingState : public GameState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void pause(GameBase &game);
			void resume(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);
			static LoadingState& getInstance() {
				static LoadingState _instance;
				return _instance;
			}

			void setIsTutorial(bool tutorial);
			void setLevel(std::string *level);
			void setSlotKeyBinding(std::map<std::string, int> *input, std::vector<std::string> *keys);

			void cleanCharacters() {
				delete _characters;
				_characters = nullptr;
			}

			void resetCharacters() {
				delete _characters;
				_characters = new std::vector<std::string>();
			}

			void addCharacter(std::string character) {
				_characters->push_back(character);
			}

		protected:
			LoadingState() { }
		private:
			GameBase *_game = nullptr;
			SDL_Thread *thread = nullptr;
			static int loadThread(void *ptr);
			void load();
			void loadCharacters(nlohmann::json &startingPositions);
			void loadBulletBobs(nlohmann::json &bobs);
			void loadTutorial();
			void loadLevel();
			void loadKeybindings();
			void clearEventQueue();
			bool _isLoaded, _isError, _isTutorial;
			std::vector<helperclasses::HUD*> *_huds = nullptr;
			std::vector<std::string> *_characters = nullptr;
			std::string *_level, *_progress = nullptr;
			std::map<std::string, int> *_slotKeyInput = nullptr;
			std::vector<std::string> *_keys = nullptr;

			// LoadingBar
			int _loadingValue, _loadingStep, _marginInner, _marginValue, _totalWidth, _totalHeight, _loadingBarX, _loadingBarY;

			bool _isAdvertisement;
			int _advertisementX, _advertisementY;
			void loadAdvertisement();
			std::string getSlotKeyInput(int slot);
		};
	}
}
