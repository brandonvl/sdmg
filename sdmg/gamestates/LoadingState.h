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
struct SDL_Thread;

using namespace sdmg::engine;

namespace JSON { class JSONArray; }
namespace sdmg {
	namespace model {
		class Platform;
		class Character;
		class MovablePlatform;
	}
	namespace helperclasses{ class HUD; }
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

		protected:
			LoadingState() { }
		private:
			SDL_Thread *thread;
			static int loadThread(void *ptr);
			void load();
			void loadCharacters(JSON::JSONArray &startingPositions);
			void loadBulletBobs(JSON::JSONArray &bobs);
			void loadTutorial();
			void loadLevel();
			void loadKeybindings();
			bool _isLoaded, _isError, _isTutorial;
			std::vector<helperclasses::HUD*> *_huds;
			std::string *_level;
			GameBase *_game;
		};
	}
}
