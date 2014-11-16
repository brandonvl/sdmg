#pragma once

#include <vector>
#include <functional>

namespace sdmg {
	namespace engine {
		class GameBase;
	}
	namespace helperclasses {
		using namespace sdmg::engine;
		class MenuItem;
		class Menu
		{
		public:
			Menu(const float x, const float y, GameBase &game) : _startingX(x), _startingY(y), _game(&game) { };
			virtual ~Menu();
			void draw(GameBase *engine);
			void addMenuTextItem(std::string text, std::function<void()> &callback);
			void removeMenuItem(MenuItem *item);
			void selectNext();
			void selectPrevious();
			void setSelected(MenuItem *selected);

			float getWidth() { return _width; }
			//void doAction();
			MenuItem *getSelectedMenuItem() { return _selected; }
		private:
			std::vector<MenuItem*> _menuItems;
			float _startingX;
			float _startingY;
			float _currentY;
			float _itemPaddingY = 5.0f;
			float _width = 375.0f;
			MenuItem *_selected;
			GameBase *_game;
		};
	}
}

