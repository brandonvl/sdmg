#pragma once
#include <string>
#include "engine\drawing\Rectangle.h"

namespace sdmg {
	namespace engine {
		namespace drawing {
			class DrawEngine;
		}
	}
	namespace model{
		class Character;
	}
	namespace helperclasses {
		using namespace engine::drawing;
		using namespace model;

		class HUD
		{
		public:
			HUD(Character &character, const float x) : _character(character), _isInitialized(false), _rectangle(x, 0, 230, 75) {}
			virtual ~HUD();
			void draw(DrawEngine &drawEngine);
		private:
			void init(DrawEngine &drawEngine);
			Character &_character;
			DrawEngine *_drawengine;
			bool _isInitialized;
			std::string _spriteKeyPrefix;
			Rectangle _rectangle;
		};

	}
}