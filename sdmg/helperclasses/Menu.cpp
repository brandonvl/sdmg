#include "Menu.h"
#include "engine\GameBase.h"
#include "MenuItem.h"

namespace sdmg {
	namespace helperclasses {

		Menu::~Menu()
		{

		}

		void Menu::addMenuItem(MenuItem *item)
		{
			if (_menuItems.empty()) {
				_selected = item;
				_selected->setSelected();
			}
			_menuItems.push_back(item);
		}

		void Menu::removeMenuItem(MenuItem *item)
		{
			//
		}

		void Menu::selectNext()
		{
			if (!_menuItems.empty()) {
				for (auto i = _menuItems.begin(); i != _menuItems.end(); ++i) {
					if (*i == _selected) {
						_selected->setSelected(false);
						_selected = *i == _menuItems.back() ? _menuItems.front() : *++i;
						_selected->setSelected();
					}
				}
			}
		}

		void Menu::selectPrevious()
		{
			if (!_menuItems.empty())
				_menuItems.reverse(); selectNext(); _menuItems.reverse();
		}
		
		void Menu::draw(GameBase *engine)
		{
			float xOffSet = _startingX, yOffSet = _startingY;

			for (auto i : _menuItems)
			{
				i->draw(engine, xOffSet, yOffSet + 10);
				yOffSet += i->getHeight() + 5.0F;
			}
		}
	}
}