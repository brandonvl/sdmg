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

			void MenuTextItem::draw(GameBase *engine, const float xOffSet, const float yOffSet)
			{
				SDL_Color fgColor;
				SDL_Color bgColor = { 255, 255, 255 };
				if (_isSelected)
					fgColor = { 255, 0, 0 };
				else
					fgColor = { 0, 0, 0 };

				engine->getEngine()->getDrawEngine()->drawText(_text, Rectangle(xOffSet, yOffSet, _width, _height),fgColor,bgColor);
			}
		}
	}
}
