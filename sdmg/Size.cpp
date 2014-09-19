#include "Size.h"

namespace SDMG {
	Size::Size(float x, float y, float width, float height)
	{
		_x = x;
		_y = y;
		_width = width;
		_height = height;
	}


	Size::~Size()
	{
	}

	float Size::getX() {
		return _x;
	}
	float Size::getY() {
		return _y;
	}
	float Size::getWidth() {
		return _width;
	}
	float Size::getHeight() {
		return _height;
	}
}