#pragma once

#include <functional>
#include <string>

namespace sdmg {
	namespace engine {
		class GameBase;
	}
	namespace helperclasses {
		using namespace sdmg::engine;
		class MenuItem
		{
		public:
			MenuItem(const float width, const float height, const bool selected = false, std::function<void(MenuItem*)> callBack = nullptr) : _width(width), _height(height), _isSelected(selected) { };
			virtual ~MenuItem() { };
			virtual void draw(GameBase *engine, const float xOffSet, const float yOffSet) = 0;
			bool isSelected() { return _isSelected; }
			float getWidth() { return _width; }
			float getHeight() { return _height; }
			void setSelected(const bool selected = true) { _isSelected = selected; }
			//bool hasCallBack() { return _callBack != nullptr; }
			//virtual void doCallBack() = 0;
			virtual std::string getTag() = 0;
		protected:
			bool _isSelected;
			float _width;
			float _height;
			//std::function<void(MenuItem*)> _callBack;
		};
	}
}

