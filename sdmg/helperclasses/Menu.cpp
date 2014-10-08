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
			/*_menuItems.push_back(item);

			if (item->isSelected()) {

			if (_selectedMenuItem != -1)
			_menuItems[_selectedMenuItem]->setSelected(false);

			_selectedMenuItem = _menuItems.size() - 1;
			}*/

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
			/*if (!_menuItems.empty())
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
			}*/
			if (!_menuItems.empty()) {
				for (auto i = _menuItems.begin(); i != _menuItems.end(); i++) {
					_selected->setSelected(false);
					_selected = *i == _selected ? *++i : *_menuItems.begin();
					_selected->setSelected();
				}
			}
		}

		void Menu::selectPrevious()
		{
			/*if (!_menuItems.empty())
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
			}*/
			if (!_menuItems.empty()) {
				_menuItems.reverse();
				for (auto i = _menuItems.begin(); i != _menuItems.end(); i++) {
						_selected->setSelected(false);
						_selected = *i == _selected ? *++i : *_menuItems.begin();
						_selected->setSelected();
				}
				_menuItems.reverse();
			}
		}
		
		void Menu::draw(GameBase *engine)
		{
			float xOffSet = _startingX, yOffSet = _startingY;

			for (auto i : _menuItems)
			{
				MenuItem *item = i;
				i->draw(engine, xOffSet, yOffSet);

				yOffSet += i->getHeight() + 1.0F;
			}
		}
	}
}