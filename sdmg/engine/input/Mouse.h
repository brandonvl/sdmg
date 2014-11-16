#pragma once
#include <vector>
#include <functional>
#include "sdl\include\SDL.h"

namespace sdmg {
	namespace engine {
		namespace input {
			class Mouse
			{
			public:
				Mouse();
				virtual ~Mouse();
				
				void handleMouseEvent(SDL_Event &event);
				void setHoverAction(float x, float y, float width, float height, std::function<void()> &hoverCallback);
				void setLeaveAction(float x, float y, float width, float height, std::function<void()> &leaveCallback);
				void setClickAction(float x, float y, float width, float height, std::function<void()> &clickCallback);
				void clear();

			private:
				struct Hitbox {
					float x, y, width, height;
					std::function<void()> callback;
				};

				std::vector<Hitbox> _clickBoxes, _hoverBoxes, _leaveBoxes;

				void execActions(std::vector<Hitbox> &boxes, int x, int y);
				bool isHit(Hitbox &box, int x, int y);
			};
		}
	}
}