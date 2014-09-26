//
//
//  @ Project : SDMG
//  @ File Name : Rectangle.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once

namespace sdmg {
	namespace engine {
		namespace drawing {
			class Rectangle {
			public:
				float getX();
				void setX(float x);
				float getY();
				void setY(float y);
				float getWidth();
				void setWidth(float width);
				float getHeight();
				void setHeight(float height);
			private:
				float _x;
				float _y;
				float _width;
				float _height;
			};
		}
	}
}