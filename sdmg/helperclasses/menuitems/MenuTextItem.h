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
				void draw(GameBase *engine, const float xOffSet, const float yOffSet) override;
				std::string getTag() { return _text; }
				//void doCallBack() { _callBack(this); }
			private:
				std::string _text;
			};
		}
	}
}

