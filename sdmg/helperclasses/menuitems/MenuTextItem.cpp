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
					_engine->getDrawEngine()->unloadText(_key);
					_engine->getDrawEngine()->unloadText(_keySelected);
				}
			}

			void MenuTextItem::loadText(GameBase *engine, std::string key, std::string text, std::string fontName, int fontSize) {
				_key = key;
				_keySelected = key + "Selected";
				_engine = engine->getEngine();
				engine->getEngine()->getDrawEngine()->loadText(_key, text, { 173, 14, 14 }, fontName, fontSize);
				engine->getEngine()->getDrawEngine()->loadText(_keySelected, text, { 255, 255, 255 }, fontName, fontSize);
			}

			void MenuTextItem::draw(GameBase *engine, const float xOffSet, const float yOffSet)
			{
				DrawEngine *drawEngine = engine->getEngine()->getDrawEngine();

				drawEngine->drawRectangle(Rectangle(xOffSet, yOffSet, WIDTH, HEIGHT), 217, 13, 13);

				float textWidth = drawEngine->getTextSize(_key)[0];
				float centeredX = (WIDTH - textWidth) / 2 + xOffSet;

				if (_isSelected)
					drawEngine->drawText(_keySelected, centeredX, yOffSet + 13);
				else
					drawEngine->drawText(_key, centeredX, yOffSet + 13);
			}
		}
	}
}
