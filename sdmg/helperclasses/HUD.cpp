#include "HUD.h"
#include "engine\drawing\DrawEngine.h"
#include "model\Character.h"

namespace sdmg {
	namespace helperclasses {
		void HUD::init(DrawEngine &drawEngine) {
			_drawengine = &drawEngine;
			_spriteKeyPrefix = "hud_" + _character->getName() + "_";

			drawEngine.load(_spriteKeyPrefix + "head", "assets/characters/" + _character->getKey() + "/head");
			drawEngine.load("heart", "assets/hud/heart");
			drawEngine.load("power", "assets/hud/power");

			_isInitialized = true;
		}

		HUD::~HUD()
		{
			if (_drawengine != nullptr) {
				_drawengine->unload(_spriteKeyPrefix + "head");
				_drawengine->unload("heart");
				_drawengine->unload("power");
			}

			_drawengine = nullptr;
			_character = nullptr;
		}

		void HUD::draw(DrawEngine &drawEngine) {

			if (!_isInitialized) init(drawEngine);

			drawEngine.drawRectangle(_rectangle, 76, 166, 199, 100);

			const int NUMLIFES = 4;
			const int HEART_WIDTH = 29;
			float heartFillWidth = _character->getHP() / 100.0f * HEART_WIDTH;

			const int xPadding = _rectangle.getX() + 75;
			for (int i = 0; i < NUMLIFES; i++) {
				int xPos = xPadding + 33 * i;

				drawEngine.drawRectangle(Rectangle(xPos, _rectangle.getY() + 15, HEART_WIDTH, 25), 51, 51, 51);

				if (_character->getLives() == (i + 1)) {
					drawEngine.drawRectangle(Rectangle(xPos, _rectangle.getY() + 15, heartFillWidth, 25), 183, 39, 39);
				}
				else if (_character->getLives() > i) {
					drawEngine.drawRectangle(Rectangle(xPos, _rectangle.getY() + 15, HEART_WIDTH, 25), 183, 39, 39);
				}

				drawEngine.draw("heart", xPos, _rectangle.getY() + 15);
			}

			const int xPowerPadding = _rectangle.getX() + 90;
			const int POWERBAR_WIDTH = 113;
			int power = _character->getPP();
			drawEngine.drawRectangle(Rectangle(xPowerPadding, _rectangle.getY() + 52, POWERBAR_WIDTH, 12), 51, 51, 51);
			drawEngine.drawRectangle(Rectangle(xPowerPadding, _rectangle.getY() + 52, (float)power / 100.0f * (float)POWERBAR_WIDTH, 12), 232, 172, 11);
			drawEngine.draw("power", xPadding, 50);

			const int xBlockPadding = _rectangle.getX() + 90;
			const int BLOCKBAR_WIDTH = 113;
			int block = _character->getBP();
			drawEngine.drawRectangle(Rectangle(xBlockPadding, _rectangle.getY() + 68, BLOCKBAR_WIDTH, 12), 51, 51, 51);
			drawEngine.drawRectangle(Rectangle(xBlockPadding, _rectangle.getY() + 68, (float)block / 100.0f * (float)BLOCKBAR_WIDTH, 12), 153, 217, 234);


			drawEngine.draw(_spriteKeyPrefix + "head", _rectangle.getX() + 10, _rectangle.getY() + 10);
		}
	}
}