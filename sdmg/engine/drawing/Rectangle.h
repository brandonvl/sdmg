//
//
//  @ Project : SDMG
//  @ File Name : Rectangle.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once

class SDL_Rect;

namespace sdmg {
	namespace engine {
		namespace drawing {
			class Rectangle {
			public:
				Rectangle(float x, float y, float width, float height);
				float getX();
				void setX(float x);
				float getY();
				void setY(float y);
				float getWidth();
				void setWidth(float width);
				float getHeight();
				void setHeight(float height);
				SDL_Rect toSDLRect();
			private:
				float _x;
				float _y;
				float _width;
				float _height;
			};
		}
	}
}