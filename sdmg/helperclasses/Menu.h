#pragma once

#include <vector>

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
			Menu(const float x, const float y) : _startingX(x), _startingY(y),_selectedMenuItem(-1) { };
			virtual ~Menu();
			void draw(GameBase *engine);
			void addMenuItem(MenuItem *item);
			void removeMenuItem(MenuItem *item);
			void selectNext();
			void selectPrevious();
			//void doAction();
			MenuItem *getSelectedMenuItem() { return _menuItems[_selectedMenuItem]; }
		private:
			std::vector<MenuItem*> _menuItems;
			float _startingX;
			float _startingY;
			int _selectedMenuItem;
		};
	}
}

