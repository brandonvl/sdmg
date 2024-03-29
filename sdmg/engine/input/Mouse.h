#pragma once
#include <vector>
#include <functional>
#include <SDL.h>

namespace sdmg {
	namespace engine {
		namespace input {
			class Mouse
			{
			public:
				struct Hitbox {
					int x, y, width, height;
					std::function<void()> callback;
				};

				Mouse();
				virtual ~Mouse();
				
				void handleMouseEvent(SDL_Event &event);
				Hitbox *setHoverAction(int x, int y, int width, int height, std::function<void()> &hoverCallback);
				Hitbox *setLeaveAction(int x, int y, int width, int height, std::function<void()> &leaveCallback);
				Hitbox *setClickAction(int x, int y, int width, int height, std::function<void()> &clickCallback);
				void setMouseUpAction(std::function<void()> &clickCallback);
				void setMouseMoveAction(std::function<void(int x, int y)> &mouseMoveCallback);
				void clear();

				int getX() { return _x; }
				int getY() { return _y; }
				std::vector<Hitbox*> &getClickBoxes();

			private:

				std::vector<Hitbox*> _clickBoxes, _hoverBoxes, _leaveBoxes;
				std::function<void(int x, int y)> _mouseMoveCallback;
				std::function<void()> _mouseUpCallback;

				void execActions(std::vector<Hitbox*> &boxes, int x, int y);
				bool isHit(Hitbox *box, int x, int y);

				int _x, _y;
			};
		}
	}
}