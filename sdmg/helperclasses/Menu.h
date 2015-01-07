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
			Menu(const int x, const int y, GameBase &game) : _startingX(x), _startingY(y), _game(&game) { };
			virtual ~Menu();
			void draw(GameBase *engine);
			void addMenuTextItem(std::string text, std::function<void()> &callback);
			void removeMenuItem(MenuItem *item);
			void selectNext();
			void selectPrevious();
			void setSelected(MenuItem *selected);
			void rebuildHitboxes();
			void doAction();

			void setWidth(int width);
			int getWidth() { return _width; }
			//void doAction();
			MenuItem *getSelectedMenuItem() { return _selected; }
		private:
			std::vector<MenuItem*> _menuItems;
			int _startingX, _startingY, _currentY, _itemPaddingY = 5, _width = 375;
			MenuItem *_selected;
			GameBase *_game;

			void buildHitbox(MenuItem *item, int y);
		};
	}
}

