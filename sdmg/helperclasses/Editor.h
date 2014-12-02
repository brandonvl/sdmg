#pragma once
#include <map>
#include <vector>
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
			class PlatformDef {
			public:
				PlatformDef(std::string name) : _name(name) {}
				
				std::string getName() { return _name; }

			private:
				std::string _name;
			};

			Editor(GameBase &game);
			virtual ~Editor();
			
			void toggle();
			void draw();
			void update();
			bool isEnabled() { return _enabled; }

		private:
			void enable();
			void disable();
			void loadPlatformDefs();
			void drawPlatforms();

			void mouseMove(int x, int y);
			void Editor::selectObject(GameObject &gameObject);

			bool _enabled;
			float _mouseDownX, _mouseDownY;
			GameObject *_curSelectedObject;
			std::map<GameObject*, input::Mouse::Hitbox*> *_hitboxes;

			SDL_Window *_window;
			SDL_Renderer *_renderer;
			GameBase *_game;

			std::vector<PlatformDef*> _platformDefs;

			const std::string PLATFORM_FOLDER = "assets/platforms/";
			const int WINDOW_WIDTH = 200, WINDOW_HEIGHT = 720;
		};
	}
}