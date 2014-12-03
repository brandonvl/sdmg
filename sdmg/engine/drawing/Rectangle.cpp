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
			Rectangle::Rectangle(int x, int y, int width, int height) : _x(x), _y(y), _width(width), _height(height) { }

			int Rectangle::getX() {
				return _x;
			}
			
			void Rectangle::setX(int x) {
				_x = x;
			}
			
			int Rectangle::getY() {
				return _y;
			}
			
			void Rectangle::setY(int y) {
				_y = y;
			}
			
			int Rectangle::getWidth() {
				return _width;
			}
			
			void Rectangle::setWidth(int width) {
				_width = width;
			}
			
			int Rectangle::getHeight() {
				return _height;
			}
			
			void Rectangle::setHeight(int height) {
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
