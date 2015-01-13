//
//
//  @ Project : SDMG
//  @ File Name : LevelSelectionState.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "SelectionState.h"
#include <map>
#include <string>
#include <vector>
#include <functional>
#include "lib\JSONParser.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace engine {
		class GameTime;
	}
	
	namespace gamestates {
		class LevelSelectionState : public SelectionState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);
			static LevelSelectionState& getInstance() {
				static LevelSelectionState _instance;
				return _instance;
			}

		protected:
			LevelSelectionState() { }
		private:
			GameBase *_game;
			std::vector<std::string> *_levels;
			int _currentLevel;

			int _xPos, _xTargetPos, _xStartPos, _xMinPos, _scrollStep = 2;
			float _step, _accumulator;
			std::chrono::high_resolution_clock::time_point _lastUpdate;

			const int
				PREVIEW_WIDTH = 500,
				PREVIEW_HEIGHT = 281,
				PREVIEW_PADDINGX = 50,
				PREVIEW_YPOS = 230,
				SCROLL_STEP_SLOW = 4,
				SCROLL_STEP_QUICK = 8,
				TEXT_YPADDING = 30;

			void drawLevel(GameBase &game);
			void selectNext();
			void selectPrevious();
			void nextState();
		};
	}
}