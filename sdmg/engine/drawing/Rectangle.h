//
//
//  @ Project : SDMG
//  @ File Name : Rectangle.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once

struct SDL_Rect;

namespace sdmg {
	namespace engine {
		namespace drawing {
			class Rectangle {
			public:
				Rectangle(int x, int y, int width, int height);
				int getX();
				void setX(int x);
				int getY();
				void setY(int y);
				int getWidth();
				void setWidth(int width);
				int getHeight();
				void setHeight(int height);
				SDL_Rect toSDLRect();
			private:
				int _x, _y, _width, _height;
			};
		}
	}
}