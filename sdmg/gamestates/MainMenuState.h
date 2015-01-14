//
//
//  @ Project : SDMG
//  @ File Name : MainMenuState.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "MenuState.h"
#include <string>
#include <vector>

using namespace sdmg::engine;

namespace sdmg {
	namespace engine {
		class GameTime;
	}

	namespace helperclasses {
		class Menu;
		class MenuItem;
	}

	namespace gamestates {
		using namespace sdmg::helperclasses;
		class MainMenuState : public MenuState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);
			
			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);

			static MainMenuState& getInstance() {
				static MainMenuState _instance;
				return _instance;
			}
		protected:
			MainMenuState() { }
		private:
			GameBase *_game;
			const int _advertisementStartX = 585;
			const int _advertisementStartY = 243;
			std::vector<std::string> *_advertismentList = nullptr;
			bool _shouldRefreshAdvertisement;
			int _advertisementIndex, _advertisementRefreshRate, _lastTimeSinceAdvertisementChange, _advertisementX, _advertisementY;
			void loadAdvertisement();
		};
	}
}