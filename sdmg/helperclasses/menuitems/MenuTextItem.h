#pragma once
#include "helperclasses\MenuItem.h"

namespace sdmg {
	namespace helperclasses {
		namespace menuitems {
			class MenuTextItem :
				public MenuItem
			{
			public:
				MenuTextItem(const std::string &text, const float width, const float height, const bool selected = false, std::function<void(MenuItem*)> callBack = nullptr) : MenuItem(width, height, selected, callBack), _text(text) { };
				virtual ~MenuTextItem();
				void loadText(GameBase *engine, std::string key, std::string text, std::string fontName, int fontSize);
				void draw(GameBase *engine, const float xOffSet, const float yOffSet) override;
				std::string getTag() { return _text; }
				//void doCallBack() { _callBack(this); }
			private:
				std::string _key;
				std::string _keySelected;
				std::string _text;
				const float WIDTH = 375, HEIGHT = 68;
			};
		}
	}
}

