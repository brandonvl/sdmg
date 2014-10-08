#pragma once

#include <list>

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
			Menu(const float x, const float y) : _startingX(x), _startingY(y) { };
			virtual ~Menu();
			void draw(GameBase *engine);
			void addMenuItem(MenuItem *item);
			void removeMenuItem(MenuItem *item);
			void selectNext();
			void selectPrevious();
			//void doAction();
			MenuItem *getSelectedMenuItem() { return _selected; }
		private:
			std::list<MenuItem*> _menuItems;
			float _startingX;
			float _startingY;
			MenuItem *_selected;
		};

	}
}

