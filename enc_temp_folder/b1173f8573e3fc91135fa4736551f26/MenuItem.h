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
			MenuItem(const int height, std::function<void()> &callback, int index) : _height(height), _callback(callback), _index(index) {};
			virtual ~MenuItem() { };
			virtual void draw(GameBase *engine, const int x, const int y, const int width) = 0;
			bool isSelected() { return _isSelected; }
			int getHeight() { return _height; }
			void setSelected(const bool selected = true) { _isSelected = selected; }
			void doAction() { _callback(); }
			virtual std::string getTag() = 0;
			const int &getIndex()  { return _index; }
			std::function<void()> &getCallback() { return _callback; }
		protected:
			bool _isSelected = false;
			int _height, _index;
			Engine *_engine;
			std::function<void()> _callback;
		};
	}
}

