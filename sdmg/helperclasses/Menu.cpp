#include "Menu.h"
#include "engine\GameBase.h"
#include "MenuItem.h"
#include "menuitems\MenuTextItem.h"
#include "engine\Engine.h"
#include "engine\input\InputEngine.h"

namespace sdmg {
	namespace helperclasses {

		Menu::~Menu()
		{
			for (MenuItem *it : _menuItems)
			{
				delete it;
			}

			_menuItems.clear();
			_selected = nullptr;
		}

		void Menu::addMenuTextItem(std::string text, std::function<void()> &callback)
		{
			MenuItem *item = new menuitems::MenuTextItem(text, callback, *_game, _menuItems.size());

			if (_menuItems.empty()) {
				_selected = item;
				_selected->setSelected();
			}
			_menuItems.push_back(item);
			buildHitbox(item, _currentY);
			_currentY += item->getHeight() + _itemPaddingY;
		}

		void Menu::doAction() {
			if (_selected != nullptr)
				_selected->doAction();
		}

		void Menu::removeMenuItem(MenuItem *item)
		{
			//
		}

		void Menu::rebuildHitboxes() {
			int currentY = 0;

			for (MenuItem *item : _menuItems) {
				buildHitbox(item, currentY);
				currentY += item->getHeight() + _itemPaddingY;
			}
		}

		void Menu::buildHitbox(MenuItem *item, int y) {
			_game->getEngine()->getInputEngine()->getMouse().setClickAction(_startingX, _startingY + y, _width, item->getHeight(), item->getCallback());
			_game->getEngine()->getInputEngine()->getMouse().setHoverAction(_startingX, _startingY + y, _width, item->getHeight(), (std::function<void()>)[&, item] { setSelected(item); });
		}

		void Menu::selectNext()
		{
			if (!_menuItems.empty()) {
				if (_selected->getIndex() < _menuItems.size() - 1) setSelected(_menuItems[_selected->getIndex() + 1]);
				else setSelected(_menuItems[0]);
			}

			/*
			if (!_menuItems.empty()) {
				for (auto i = _menuItems.begin(); i != _menuItems.end(); ++i) {
					if (*i == _selected) {
						_selected->setSelected(false);
						_selected = *i == _menuItems.back() ? _menuItems.front() : *++i;
						_selected->setSelected();
					}
				}
			}*/
		}

		void Menu::selectPrevious()
		{
			if (!_menuItems.empty()) {
				if (_selected->getIndex() > 0) setSelected(_menuItems[_selected->getIndex() - 1]);
				else setSelected(_menuItems[_menuItems.size() - 1]);
			}
		}

		void Menu::setSelected(MenuItem *selected) {
			_selected->setSelected(false);
			_selected = selected;
			_selected->setSelected();
		}
		
		void Menu::draw(GameBase *engine)
		{
			int xOffSet = _startingX, yOffSet = _startingY;

			for (auto &i : _menuItems)
			{
				i->draw(engine, xOffSet, yOffSet, _width);
				yOffSet += i->getHeight() + _itemPaddingY;
			}
		}

		void Menu::setWidth(int width)
		{
			_width = width;
		}
	}
}