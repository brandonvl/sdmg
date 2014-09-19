#pragma once

namespace SDMG {
	class Size
	{
	public:
		Size(float x, float y, float width, float height);
		~Size();

		float getX();
		float getY();
		float getWidth();
		float getHeight();
	private:
		float _x, _y, _width, _height;
	};
}
