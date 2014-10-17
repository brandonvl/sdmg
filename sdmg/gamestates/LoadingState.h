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

using namespace sdmg::engine;

namespace sdmg {
	namespace model {
		class Platform;
		class Character;
		class MovablePlatform;
	}
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

		protected:
			LoadingState() { }
		private:
			static int loadThread(void *ptr);
			void load();
			bool _isLoaded, _isError;
			std::vector<model::Character*> *_characters;
			std::vector<model::MovablePlatform*> *_bullets;
			model::Platform *_platform;
			GameBase *_game;
		};
	}
}
