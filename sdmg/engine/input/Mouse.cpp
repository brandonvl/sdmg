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
				}

				if (event.type == SDL_MOUSEBUTTONDOWN) {
					int x, y;
					SDL_GetMouseState(&x, &y);
					execActions(_clickBoxes, x, y);
				}
			}

			void Mouse::setHoverAction(float x, float y, float width, float height, std::function<void()> &hoverCallback) {
				_hoverBoxes.push_back({ x, y, width, height, hoverCallback });
			}
			
			void Mouse::setLeaveAction(float x, float y, float width, float height, std::function<void()> &leaveCallback) {
				_leaveBoxes.push_back({ x, y, width, height, leaveCallback });
			}

			void Mouse::setClickAction(float x, float y, float width, float height, std::function<void()> &clickCallback) {
				_clickBoxes.push_back({ x, y, width, height, clickCallback });
			}

			void Mouse::execActions(std::vector<Hitbox> &boxes, int x, int y) {
				for (Hitbox box : boxes) {
					if (isHit(box, x, y)) {
						box.callback();
						return;
					}
				}
			}
			
			bool Mouse::isHit(Hitbox &box, int x, int y) {
				return (x >= box.x && x <= box.x + box.width &&
					y >= box.y && y <= box.y + box.height);
			}

			void Mouse::clear() {
				_hoverBoxes.clear();
				_leaveBoxes.clear();
				_clickBoxes.clear();
			}
		}
	}
}