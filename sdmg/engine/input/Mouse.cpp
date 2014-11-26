#include "Mouse.h"

namespace sdmg {
	namespace engine {
		namespace input {
			Mouse::Mouse()
			{
			}


			Mouse::~Mouse()
			{
			}

			void Mouse::handleMouseEvent(SDL_Event &event) {
				if (event.type == SDL_MOUSEMOTION) {
					int x, y;
					SDL_GetMouseState(&x, &y);
					execActions(_hoverBoxes, x, y);

					if (_mouseMoveCallback)
						_mouseMoveCallback(x, y);

					_x = x;
					_y = y;
				}

				if (event.type == SDL_MOUSEBUTTONDOWN) {
					int x, y;
					SDL_GetMouseState(&x, &y);
					execActions(_clickBoxes, x, y);
				}

				if (event.type == SDL_MOUSEBUTTONUP) {
					if (_mouseUpCallback)
						_mouseUpCallback();
				}
			}

			std::vector<Mouse::Hitbox*> &Mouse::getClickBoxes() { return _clickBoxes; }

			Mouse::Hitbox *Mouse::setHoverAction(float x, float y, float width, float height, std::function<void()> &hoverCallback) {
				_hoverBoxes.push_back(new Hitbox{ x, y, width, height, hoverCallback });
				return _hoverBoxes[_hoverBoxes.size() - 1];
			}
			
			Mouse::Hitbox *Mouse::setLeaveAction(float x, float y, float width, float height, std::function<void()> &leaveCallback) {
				_leaveBoxes.push_back(new Hitbox{ x, y, width, height, leaveCallback });
				return _leaveBoxes[_leaveBoxes.size() - 1];
			}

			Mouse::Hitbox *Mouse::setClickAction(float x, float y, float width, float height, std::function<void()> &clickCallback) {
				_clickBoxes.push_back(new Hitbox{ x, y, width, height, clickCallback });
				return _clickBoxes[_clickBoxes.size() - 1];
			}

			void Mouse::setMouseUpAction(std::function<void()> &mouseUpCallback) {
				_mouseUpCallback = mouseUpCallback;
			}

			void Mouse::setMouseMoveAction(std::function<void(int x, int y)> &mouseMoveCallback) {
				_mouseMoveCallback = mouseMoveCallback;
			};

			void Mouse::execActions(std::vector<Hitbox*> &boxes, int x, int y) {
				for (Hitbox *box : boxes) {
					if (isHit(box, x, y)) {
						box->callback();
						return;
					}
				}
			}
			
			bool Mouse::isHit(Hitbox *box, int x, int y) {
				return (x >= box->x && x <= box->x + box->width &&
					y >= box->y && y <= box->y + box->height);
			}

			void Mouse::clear() {
				for (auto box : _hoverBoxes)
					delete box;

				_hoverBoxes.clear();

				for (auto box : _leaveBoxes)
					delete box;
				_leaveBoxes.clear();

				for (auto box : _clickBoxes)
					delete box;
				_clickBoxes.clear();
			}
		}
	}
}