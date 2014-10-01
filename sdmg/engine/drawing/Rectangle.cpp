//
//
//  @ Project : SDMG
//  @ File Name : Rectangle.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "Rectangle.h"
#include "..\..\sdl\include\SDL.h"

namespace sdmg {
	namespace engine {
		namespace drawing {
			Rectangle::Rectangle(float x, float y, float width, float height) {
				setX(x);
				setY(y);
				setWidth(width);
				setHeight(height);
			}

			float Rectangle::getX() {
				return _x;
			}
			
			void Rectangle::setX(float x) {
				_x = x;
			}
			
			float Rectangle::getY() {
				return _y;
			}
			
			void Rectangle::setY(float y) {
				_y = y;
			}
			
			float Rectangle::getWidth() {
				return _width;
			}
			
			void Rectangle::setWidth(float width) {
				_width = width;
			}
			
			float Rectangle::getHeight() {
				return _height;
			}
			
			void Rectangle::setHeight(float height) {
				_height = height;
			}

			SDL_Rect Rectangle::toSDLRect() {
				SDL_Rect rect;
				rect.x = _x;
				rect.y = _y;
				rect.w = _width;
				rect.h = _height;		
				
				return rect;
			}			
		}
	}
}
