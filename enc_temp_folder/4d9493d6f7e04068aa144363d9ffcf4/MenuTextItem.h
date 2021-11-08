#pragma once
#include "helperclasses\MenuItem.h"

namespace sdmg {
	namespace helperclasses {
		namespace menuitems {
			class MenuTextItem :
				public MenuItem
			{
			public:
				MenuTextItem(const std::string &text, std::function<void()> &callback, GameBase &game, int index) : MenuItem(68, callback, index), _text(text) { loadText(game); };
				virtual ~MenuTextItem();
				void loadText(GameBase &game);
				void setText(const std::string &text, GameBase &game);
				void draw(GameBase *engine, const int x, const int y, const int width) override;
				std::string getTag() { return _text; }
				//void doCallBack() { _callBack(this); }
			private:
				std::string _text;
				const char* FONTNAME = "trebucbd";
				const int FONTSIZE = 33;
			};
		}
	}
}

