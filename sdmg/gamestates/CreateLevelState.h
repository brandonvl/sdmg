#pragma once
#include "SelectionState.h"
#include <string>

using namespace sdmg::engine;

namespace sdmg {
	namespace gamestates {
		class CreateLevelState : public SelectionState
		{
		public:
			virtual void init(GameBase &game);
			virtual void cleanup(GameBase &game);

			virtual void pause(GameBase &game);
			virtual void resume(GameBase &game);

			virtual void handleEvents(GameBase &game, GameTime &gameTime);
			virtual void update(GameBase &game, GameTime &gameTime);
			virtual void draw(GameBase &game, GameTime &gameTime);
			static CreateLevelState& getInstance() {
				static CreateLevelState _instance;
				return _instance;
			}
			
		private:
			std::string *_name;
			GameBase *_game;
			std::string *_error;

			void create();
		};

	}
}