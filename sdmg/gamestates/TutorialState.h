//
//
//  @ Project : SDMG
//  @ File Name : TutortialState.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "engine\GameState.h"
#include "sdl\include\SDL.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace model {
		class Platform;
		class Character;
		class MovablePlatform;
	}
	namespace gamestates {
		class TutorialState : public GameState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void pause(GameBase &game);
			void resume(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);

			static TutorialState& getInstance() {
				static TutorialState _instance;
				return _instance;
			}

			void setCharacters(std::vector<model::Character*> *characters);
			void setPlatform(model::Platform *platform);

		protected:
			TutorialState() { }
		private:
			std::vector<model::Character*> *_characters;
			model::Platform *_platform;

			std::vector<std::pair<SDL_Keycode, std::string>> *_tutorial;
			std::vector<std::string> *_toDraw;
			SDL_Keycode _pressed;
		};
	}
}

