#include "MenuTextItem.h"
#include "engine\GameBase.h"
#include "engine\Engine.h"
#include "engine\drawing\DrawEngine.h"


namespace sdmg {
	namespace helperclasses {
		namespace menuitems {

			MenuTextItem::~MenuTextItem()
			{
				if (_engine != nullptr) {
					_engine->getDrawEngine()->unloadText("MI_" + _text);
					_engine->getDrawEngine()->unloadText("MI_" + _text + "_SEL");
				}
			}

			void MenuTextItem::loadText(GameBase &game) {
				game.getEngine()->getDrawEngine()->loadText("MI_" + _text, _text, { 173, 14, 14 }, FONTNAME, FONTSIZE);
				game.getEngine()->getDrawEngine()->loadText("MI_" + _text + "_SEL", _text, { 255, 255, 255 }, FONTNAME, FONTSIZE);
			}

			void MenuTextItem::draw(GameBase *engine, const float x, const float y, const float width)
			{
				DrawEngine *drawEngine = engine->getEngine()->getDrawEngine();

				drawEngine->drawRectangle(Rectangle(x, y, width, _height), 217, 13, 13);

				float textWidth = drawEngine->getTextSize("MI_" + _text)[0];
				float centeredX = (width - textWidth) / 2 + x;

				if (_isSelected)
					drawEngine->drawText("MI_" + _text + "_SEL", centeredX, y + 13);
				else
					drawEngine->drawText("MI_" + _text, centeredX, y + 13);
			}
		}
	}
}
