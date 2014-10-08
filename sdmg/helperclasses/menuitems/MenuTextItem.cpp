#include "MenuTextItem.h"
#include "engine\GameBase.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"


namespace sdmg {
	namespace helperclasses {
		namespace menuitems {

			MenuTextItem::~MenuTextItem()
			{
			}

			void MenuTextItem::loadText(GameBase *engine, std::string key, std::string text, std::string fontName, int fontSize) {
				_key = key;
				_keySelected = key + "Selected";
				engine->getEngine()->getDrawEngine()->loadText(_key, text, { 0, 0, 0 }, { 255, 255, 255 }, fontName, fontSize);
				engine->getEngine()->getDrawEngine()->loadText(_keySelected, text, { 255, 0, 0 }, { 255, 255, 255 }, fontName, fontSize);
			}

			void MenuTextItem::draw(GameBase *engine, const float xOffSet, const float yOffSet)
			{
				if (_isSelected)
					engine->getEngine()->getDrawEngine()->drawText(_keySelected, Rectangle(xOffSet, yOffSet, _width, _height));
				else
					engine->getEngine()->getDrawEngine()->drawText(_key, Rectangle(xOffSet, yOffSet, _width, _height));
			}
		}
	}
}
