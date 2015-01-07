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
#include "lib\JSONParser.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace gamestates {
		class CharacterSelectionState : public SelectionState {
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);
			
			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);
			static CharacterSelectionState& getInstance() {
				static CharacterSelectionState _instance;
				return _instance;
			}
		protected:
			CharacterSelectionState() { }
		private:
			GameBase *_game;
			std::map<std::string, JSON::JSONDocument*> *_characters;
			std::vector<std::string> *_slots;
			std::string *_currentCharacter;

			void drawCharacters(GameBase &game);
			void drawSelectedCharacters(GameBase &game);
		};
	}
}