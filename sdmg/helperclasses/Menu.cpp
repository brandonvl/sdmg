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
			_menuItems.push_back(item);

			if (item->isSelected()) {

				if (_selectedMenuItem != -1)
					_menuItems[_selectedMenuItem]->setSelected(false);

				_selectedMenuItem = _menuItems.size() - 1;
			}
		}

		void Menu::removeMenuItem(MenuItem *item)
		{
			//
		}

		void Menu::selectNext()
		{
			if (!_menuItems.empty())
			{
				if (_selectedMenuItem == -1 || _selectedMenuItem == _menuItems.size() -1) {

					if (_selectedMenuItem != -1)
						_menuItems[_selectedMenuItem]->setSelected(false);

					_selectedMenuItem = 0;
					_menuItems[_selectedMenuItem]->setSelected();
				}
				else
				{
					_menuItems[_selectedMenuItem]->setSelected(false);
					_selectedMenuItem++;
					_menuItems[_selectedMenuItem]->setSelected();
				}
			}
		}

		void Menu::selectPrevious()
		{
			if (!_menuItems.empty())
			{
				if (_selectedMenuItem == -1 || _selectedMenuItem == 0) {

					if (_selectedMenuItem != -1)
						_menuItems[_selectedMenuItem]->setSelected(false);

					_selectedMenuItem = _menuItems.size() - 1;
					_menuItems[_selectedMenuItem]->setSelected();
				}
				else
				{
					_menuItems[_selectedMenuItem]->setSelected(false);
					_selectedMenuItem--;
					_menuItems[_selectedMenuItem]->setSelected();
				}
			}
		}

		void Menu::draw(GameBase *engine)
		{
			float xOffSet = _startingX, yOffSet = _startingY;

			for (auto item : _menuItems)
			{
				item->draw(engine, xOffSet, yOffSet);

				yOffSet += item->getHeight() + 1.0F;
			}
		}
	}
}