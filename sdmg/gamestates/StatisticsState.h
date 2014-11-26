#pragma once
#include "engine\GameState.h"

using  namespace sdmg::engine;

namespace JSON { class JSONDocument; }
namespace sdmg {
	namespace gamestates {
		class StatisticsState :
			public GameState
		{
		public:
			void init(GameBase &game);
			void cleanup(GameBase &game);

			void pause(GameBase &game);
			void resume(GameBase &game);

			void handleEvents(GameBase &game, GameTime &gameTime);
			void update(GameBase &game, GameTime &gameTime);
			void draw(GameBase &game, GameTime &gameTime);

			static StatisticsState& getInstance() {
				static StatisticsState _instance;
				return _instance;
			}

		private:
			StatisticsState() { }
			void loadText(std::string key, std::string text, std::string fontName, int fontSize);

			GameBase *_game;
		};
	}
}