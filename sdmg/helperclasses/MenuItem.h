#pragma once

#include <functional>
#include <string>

namespace sdmg {
	namespace engine {
		class Engine;
		class GameBase;
	}
	namespace helperclasses {
		using namespace sdmg::engine;
		class MenuItem
		{
		public:
			MenuItem(const float height, std::function<void()> &callback, int index) : _height(height), _callback(callback), _index(index) {};
			virtual ~MenuItem() { };
			virtual void draw(GameBase *engine, const float x, const float y, const float width) = 0;
			bool isSelected() { return _isSelected; }
			float getHeight() { return _height; }
			void setSelected(const bool selected = true) { _isSelected = selected; }
			void doAction() { _callback(); }
			virtual std::string getTag() = 0;
			const int &getIndex()  { return _index; }
			std::function<void()> &getCallback() { return _callback; }
		protected:
			bool _isSelected = false;
			float _height;
			int _index;
			Engine *_engine;
			std::function<void()> _callback;
		};
	}
}

