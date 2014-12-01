#pragma once
#include <map>
#include "engine\GameObject.h"
#include "engine\input\Mouse.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace engine{
		class GameBase;
	}
	namespace helperclasses {
		class Editor
		{
		public:
			Editor(GameBase &game);
			virtual ~Editor();
			
			void toggle();
			void draw();
			void update();
			bool isEnabled() { return _enabled; }

		private:
			void enable();
			void disable();

			void mouseMove(int x, int y);
			void Editor::selectObject(GameObject &gameObject);

			bool _enabled;
			float _mouseDownX, _mouseDownY;
			GameObject *_curSelectedObject;
			std::map<GameObject*, input::Mouse::Hitbox*> *_hitboxes;

			SDL_Window *_window;
			SDL_Renderer *_renderer;
			GameBase *_game;
		};
	}
}