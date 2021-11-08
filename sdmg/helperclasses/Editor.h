#pragma once
#include <map>
#include <vector>
#include "engine\GameObject.h"
#include "engine\input\Mouse.h"
#include <functional>
#include <SDL_ttf.h>

using namespace sdmg::engine;

namespace sdmg {
	namespace engine{
		class GameBase;
		class GameObject;
	}
	namespace helperclasses {
		class Editor
		{
		public:
			class PlatformDef {
			public:
				PlatformDef(Editor &editor, std::string name) : _editor(&editor), _name(name) { load(); }
				~PlatformDef() {
					SDL_DestroyTexture(_textTexture);
					SDL_DestroyTexture(_texture);
				}
								
				std::string getName() { return _name; }
				SDL_Texture *getTexture() { return _texture; }
				SDL_Texture *getTextTexture() { return _textTexture; }
				const int getTextWidth() { return _width; }
				const int getTextHeight()  { return _height; }
				SDL_Surface *getBlock(std::string name) { return _blocks[name]; }
				const int getMinWidth();
				const int getHeight();
				SDL_Surface *getSurface(int width);
				int calcWidth(int width);

			private:
				std::string _name;
				void load();
				SDL_Texture *_texture = nullptr, *_textTexture = nullptr;
				std::map<std::string, SDL_Surface*> _blocks;
				Editor *_editor = nullptr;
				int _width, _height;
			};

			class ToolbarButton {
			public:
				ToolbarButton(Editor &editor, std::string image);
				void draw(int x, int y);
				bool isSelected() { return _editor->_currentToolbarButton == this; }
				void setClickAction(std::function<void()> action) { _clickAction = action; }
				void setMouseDownOnLevelAction(std::function<void()> action) { _mouseDownOnLevelAction = action; }
				void setMouseMoveOnLevelAction(std::function<void(int x, int y)> action) { _mouseMoveOnLevelAction = action; }
				void setMouseUpOnLevelAction(std::function<void()> action) { _mouseUpOnLevelAction = action; }

				void click() { if (_clickAction) _clickAction(); }
				void mouseDownOnLevel() { if (_mouseDownOnLevelAction) _mouseDownOnLevelAction(); }
				void mouseMoveOnLevel(int x, int y) { if (_mouseMoveOnLevelAction) _mouseMoveOnLevelAction(x, y); }
				void mouseUpOnLevel() { if (_mouseUpOnLevelAction) _mouseUpOnLevelAction(); }

			private:
				SDL_Texture *_texture = nullptr;
				Editor *_editor = nullptr;
				std::function<void()> _clickAction, _mouseDownOnLevelAction, _mouseUpOnLevelAction;
				std::function<void(int x, int y)> _mouseMoveOnLevelAction;
			};

			Editor(GameBase &game);
			virtual ~Editor();
			
			void toggle();
			void draw();
			void update();
			void handleEvent(SDL_Event &event);
			bool isEnabled() { return _enabled; }
			void save(std::string name);

			void setLevel(std::string name) { _levelName = name; }

		private:
			void enable();
			void disable();
			void loadPlatformDefs();
			void drawPlatforms();
			void drawMouseBlock();
			void drawToolbar();
			void createPlatform();
			void createHitbox(GameObject *obj);
			void loadToolbox();

			void mouseMove(int x, int y);
			void Editor::selectObject(GameObject &gameObject);

			std::string _levelName;
			bool _enabled = false;
			int _mouseDownX, _mouseDownY;
			GameObject *_curSelectedObject = nullptr;
			std::map<GameObject*, input::Mouse::Hitbox*> *_hitboxes = nullptr;

			SDL_Window *_window = nullptr;
			SDL_Renderer *_renderer = nullptr;
			GameBase *_game = nullptr;
			TTF_Font* _font = nullptr;;
			PlatformDef* _currentPlatformDef = nullptr;
			ToolbarButton *_currentToolbarButton = nullptr, *_editButton = nullptr, *_moveButton = nullptr, *_eraserButton = nullptr, *_saveButton = nullptr;

			std::vector<PlatformDef*> _platformDefs;
			std::vector<ToolbarButton*> _buttons;
			std::map<GameObject*, SDL_Surface*> _drawnPlatformSurfaces;

			const std::string PLATFORM_FOLDER = "assets/platforms/", ICONS_FOLDER = "assets/editor/";
			const int WINDOW_WIDTH = 200, WINDOW_HEIGHT = 720, ITEM_HEIGHT = 40;
		};
	}
}