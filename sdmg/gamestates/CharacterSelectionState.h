//
//
//  @ Project : SDMG
//  @ File Name : CharacterSelectionState.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "SelectionState.h"
#include <map>
#include <string>
#include <vector>
#include <json.hpp>

using namespace sdmg::engine;

namespace sdmg {
	namespace gamestates {
		class CharacterSelectionState : public SelectionState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);
			
			void pause(GameBase &game) override {}
			void resume(GameBase &game) override {}

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);
			static CharacterSelectionState& getInstance() {
				static CharacterSelectionState _instance;
				return _instance;
			}

			std::string getSlotKeyInput(int slot);
		protected:
			CharacterSelectionState() { }
		private:
			GameBase *_game;
			std::map<std::string, nlohmann::json> *_characters, *_lockedCharacters;
			std::vector<std::string> *_slots;
			std::string *_currentCharacter;
			std::map<std::string, int> *_slotKeyInput;
			std::vector<std::string> *_keys;

			void loadKeys();
			void drawCharacters(GameBase &game);
			void drawSelectedCharacters(GameBase &game);
			void selectNext();
			void selectPrevious();
			void select(std::string device);
			void nextState();
			void removeLast();
			bool hasForDoubleControllers();

			int
				SELECTED_CHARACTER_BOX_WIDTH = 100;

			const int
				SELECTED_CHARACTER_BOX_PADDING = 10,
				SELECTED_CHARACTER_BOX_HEIGHT = 300,
				SELECTED_CHARACTER_BOX_YPOS = 230,
				SMALL_CHARACTER_BOX_PADDING = 10,
				SMALL_CHARACTER_BOX_WIDTH = 100,
				SMALL_CHARACTER_BOX_HEIGHT = 100,
				SMALL_CHARACTER_BOX_YPOS = 540;
		};
	}
}