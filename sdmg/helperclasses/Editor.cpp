#include "Editor.h"
#include "engine\GameBase.h"
#include "engine\Engine.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\input\InputEngine.h"
#include "engine\input\Mouse.h"
#include "engine\World.h"
#include "engine\util\FileManager.h"
#include <sdl\include\SDL_image.h>
#include <sdl\include\SDL.h>
#include "model\Platform.h"
#include "lib\JSONParser.h"

namespace sdmg {
	namespace helperclasses {
		

		Editor::Editor(GameBase &game)
		{
			_game = &game;
			_hitboxes = new std::map<GameObject*, input::Mouse::Hitbox*>();

			_game->getEngine()->getDrawEngine()->loadText("editmode", "Edit mode", { 255, 255, 255 }, "arial", 18);
			_font = TTF_OpenFont("assets/fonts/arial.ttf", 14);
		}
		
		Editor::~Editor()
		{
			delete _hitboxes;
			_hitboxes = nullptr;
			TTF_CloseFont(_font);

			_game->getEngine()->getDrawEngine()->unloadText("editmode");
		}

		void Editor::toggle() {
			if (_enabled) disable();
			else enable();
		}

		void Editor::enable() {
			_enabled = true;
			_game->getEngine()->getPhysicsEngine()->pause();

			InputEngine *inputEngine = _game->getEngine()->getInputEngine();

			for (auto obj : _game->getWorld()->getGameObjects()) {
				createHitbox(obj);
			}

			inputEngine->getMouse().setMouseMoveAction((std::function<void(int x, int y)>)[&](int x, int y) { mouseMove(x, y); });
			inputEngine->getMouse().setMouseUpAction((std::function<void()>)[&] { _curSelectedObject = nullptr; });
			inputEngine->setMouseEnabled(true);

			_window = SDL_CreateWindow("Elements", 10, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
			_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

			loadToolbox();
			loadPlatformDefs();
		}

		void Editor::loadToolbox() {
			_buttons.push_back(_editButton = new ToolbarButton(*this, "edit"));
			_editButton->setClickAction([&] {
				if (_platformDefs.size() > 0)
					_currentPlatformDef = _platformDefs[0];
			});
			_editButton->setMouseDownOnLevelAction([&] {
				if (_currentPlatformDef) {
					int x, y;
					SDL_GetMouseState(&x, &y);
					_mouseDownX = x;
				}
			});
			_editButton->setMouseUpOnLevelAction([&] {
				if (_currentPlatformDef) {
					createPlatform();
					_mouseDownX = -1;
				}
			});

			_buttons.push_back(_moveButton = new ToolbarButton(*this, "move"));
			_moveButton->setClickAction([&] { _currentPlatformDef = nullptr; });
			_moveButton->setMouseMoveOnLevelAction([&](int x, int y) {
				if (_curSelectedObject) {
					_curSelectedObject->getBody()->SetTransform(b2Vec2((x - _mouseDownX) / 20.f, (y - _mouseDownY) / 20.0f), _curSelectedObject->getBody()->GetAngle());

					if (_hitboxes->count(_curSelectedObject)) {
						input::Mouse::Hitbox *hitbox = _hitboxes->at(_curSelectedObject);
						hitbox->x = _curSelectedObject->getPixelX() - _curSelectedObject->getWidth() / 2;
						hitbox->y = _curSelectedObject->getPixelY() - _curSelectedObject->getHeight() / 2;
					}
				}
			});

			_buttons.push_back(_eraserButton = new ToolbarButton(*this, "eraser"));
			_eraserButton->setMouseDownOnLevelAction([&]() {
				_curSelectedObject->getBody()->GetWorld()->DestroyBody(_curSelectedObject->getBody());
				_game->getWorld()->removePlatform(_curSelectedObject);			
			});
			_eraserButton->setClickAction([&] { _currentPlatformDef = nullptr; });

			_buttons.push_back(_saveButton = new ToolbarButton(*this, "save"));
			_saveButton->setClickAction([&] { save(_levelName); });

			// set default selected button
			_currentToolbarButton = _moveButton;
		}

		void Editor::disable() {
			_enabled = false;
			_hitboxes->clear();

			_game->getEngine()->getInputEngine()->getMouse().clear();
			_game->getEngine()->getInputEngine()->setMouseEnabled(false);
			_game->getEngine()->getPhysicsEngine()->resume();

			SDL_DestroyRenderer(_renderer);
			SDL_DestroyWindow(_window);

			for (auto def : _platformDefs) {
				delete def;
			}
			_platformDefs.clear();
			_currentPlatformDef = nullptr;

			for (auto btn : _buttons)
				delete btn;
			_buttons.clear();
		}
		
		void Editor::loadPlatformDefs() {

			std::vector<std::string> folders = engine::util::FileManager::getInstance().getFolders(PLATFORM_FOLDER);

			for (auto folder : folders) {
				_platformDefs.push_back(new PlatformDef(*this, folder));				
			}

		}

		void Editor::handleEvent(SDL_Event &event) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_F4 ||
					event.key.keysym.sym == SDLK_ESCAPE)
					disable();
			}
			else if (event.window.windowID == SDL_GetWindowID(_window)) {
				if (event.type == SDL_MOUSEBUTTONDOWN) {
					int x, y;
					SDL_GetMouseState(&x, &y);

					if (y > WINDOW_HEIGHT - 32) {
						int buttonIndex = x / 32;
						if (buttonIndex < _buttons.size()) {
							_currentToolbarButton = _buttons[buttonIndex];
							_currentToolbarButton->click();
						}
					}
					else {
						uint32 i = (int)floor(y / 40.0f);
						if (i < _platformDefs.size()) {
							_currentPlatformDef = _platformDefs[i];
							_currentToolbarButton = _editButton;
						}
						else {
							_currentPlatformDef = nullptr;
							_currentToolbarButton = _moveButton;
						}

						_mouseDownX = -1;
					}
				}
			}
			else if (_currentPlatformDef == nullptr) {
				_game->getEngine()->getInputEngine()->handleEvent(event);
			}
			else if (_currentToolbarButton && event.window.windowID != SDL_GetWindowID(_window)) {
				if (event.type == SDL_MOUSEBUTTONDOWN) 
					_currentToolbarButton->mouseDownOnLevel();
				else if (event.type == SDL_MOUSEBUTTONUP)
					_currentToolbarButton->mouseUpOnLevel();
			}
		}

		void Editor::createHitbox(GameObject *obj) {
			_hitboxes->insert(std::make_pair(obj, _game->getEngine()->getInputEngine()->getMouse().setClickAction((int)(obj->getPixelX() - obj->getWidth() / 2), (int)(obj->getPixelY() - obj->getHeight() / 2), obj->getWidth(), obj->getHeight(), (std::function<void()>)[&, obj] { selectObject(*obj); })));
		}

		void Editor::createPlatform(){
			int x, y, w, h = _currentPlatformDef->getHeight();
			SDL_GetMouseState(&x, &y);
			
			w = _currentPlatformDef->calcWidth(x - _mouseDownX);
			int posX = (int)(_mouseDownX + w / 2);
			
			model::Platform *platform = new model::Platform();
			platform->setSize((float)w, (float)h);
			platform->setLocation((float32)posX, (float32)y);

			int bodyPaddingX = 30, bodyPaddingY = 20;

			_game->getEngine()->getPhysicsEngine()->addBody(platform, (float)bodyPaddingX, (float)bodyPaddingY);
			_game->getWorld()->addPlatform(platform);

			SDL_Surface *surface = _currentPlatformDef->getSurface(w);
			_game->getEngine()->getDrawEngine()->load(platform, surface);
			_drawnPlatformSurfaces.insert(std::make_pair(platform, surface));

			createHitbox(platform);
		}

		void Editor::update() {

		}

		void Editor::draw() {
			if (_enabled) {
				_game->getEngine()->getDrawEngine()->drawRectangle(Rectangle(0, 0, 1920, 40), 0, 0, 0);
				_game->getEngine()->getDrawEngine()->drawText("editmode", 10, 10);

				SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 0);
				SDL_RenderClear(_renderer);

				drawPlatforms();		
				drawMouseBlock();
				drawToolbar();
				
				SDL_RenderPresent(_renderer);
			}
		}

		void Editor::drawToolbar() {

			int toolbarY = WINDOW_HEIGHT - 32;
			SDL_SetRenderDrawColor(_renderer, 220, 220, 220, 0);

			SDL_Rect toolbarRect{ 0, toolbarY, WINDOW_WIDTH, 32 };
			SDL_RenderFillRect(_renderer, &toolbarRect);

			for (int i = 0; i < _buttons.size(); i++)
				_buttons[i]->draw(i * 32, toolbarY);
		}

		void Editor::drawPlatforms() {
			int yPos = 0;

			SDL_SetRenderDrawColor(_renderer, 200, 200, 200, 0);
			for (auto def : _platformDefs) {

				// draw selectionbox if this is the current def
				if (_currentPlatformDef == def) {
					SDL_SetRenderDrawColor(_renderer, 94, 140, 151, 0);
					SDL_Rect rect{ 0, yPos, WINDOW_WIDTH, ITEM_HEIGHT }; // { x, y, width, height }
					SDL_RenderFillRect(_renderer, &rect);
					SDL_SetRenderDrawColor(_renderer, 200, 200, 200, 0);
				}

				SDL_Rect previewRect{ 4, yPos + 4, 32, 32 };
				SDL_RenderCopy(_renderer, def->getTexture(), nullptr, &previewRect);
				
				SDL_Rect textRect{ 43, yPos + 12, def->getTextWidth(), def->getTextHeight() };
				SDL_RenderCopy(_renderer, def->getTextTexture(), nullptr, &textRect);

				yPos += ITEM_HEIGHT;
				SDL_RenderDrawLine(_renderer, 0, yPos - 1, WINDOW_WIDTH, yPos - 1);
			}
		}

		void Editor::drawMouseBlock() {
			if (_currentPlatformDef != nullptr && _window != SDL_GetMouseFocus()) {
				int x, y, w = 0, h = _currentPlatformDef->getHeight();
				SDL_GetMouseState(&x, &y);

				if (_mouseDownX > 0) 
					w = x - _mouseDownX;

				w = _currentPlatformDef->calcWidth(w);

				_game->getEngine()->getDrawEngine()->drawRectangle(Rectangle(_mouseDownX > 0 ? _mouseDownX : x, y - h / 2, w, h), 100, 100, 100);
			}
		}

		void Editor::mouseMove(int x, int y) {
			if (_currentToolbarButton)
				_currentToolbarButton->mouseMoveOnLevel(x,y);			
		}

		void Editor::selectObject(GameObject &gameObject) {
			_mouseDownX = _game->getEngine()->getInputEngine()->getMouse().getX() - gameObject.getPixelX();
			_mouseDownY = _game->getEngine()->getInputEngine()->getMouse().getY() - gameObject.getPixelY();
			_curSelectedObject = &gameObject;

			if (_currentToolbarButton)
				_currentToolbarButton->mouseDownOnLevel();
		}

		void Editor::PlatformDef::load() {
			SDL_Surface *surface = IMG_Load((_editor->PLATFORM_FOLDER + _name + "/preview").c_str());
			_texture = SDL_CreateTextureFromSurface(_editor->_renderer, surface);
			SDL_FreeSurface(surface);

			SDL_Surface *textSurface = TTF_RenderText_Solid(_editor->_font, _name.c_str(), SDL_Color{ 0, 0, 0 });
			_textTexture = SDL_CreateTextureFromSurface(_editor->_renderer, textSurface);
			_width = textSurface->w;
			_height = textSurface->h;
			SDL_FreeSurface(textSurface);


			std::vector<std::string> files = util::FileManager::getInstance().getFiles(_editor->PLATFORM_FOLDER + _name + "/");

			for (auto file : files) {
				if (file.size() > 6 && file.substr(file.size() - 6, 6) == ".block") {
					_blocks.insert(std::make_pair(file.substr(0, file.size() - 6), IMG_Load((_editor->PLATFORM_FOLDER + _name + "/" + file).c_str())));
				}
			}

			if (_blocks.count("basic") < 1) {
				printf("'%s' should at least contain a basic block.\n", _name);
				return;
			}
		}

		const int Editor::PlatformDef::getHeight() {
			return _blocks["basic"]->h;
		}

		const int Editor::PlatformDef::getMinWidth() {
			int w = 0;
			
			if (auto left = _blocks["left"]) w += left->w;
			if (auto right = _blocks["right"]) w += right->w;

			if (w == 0) return _blocks["basic"]->w;
			return w;
		}

		SDL_Surface *Editor::PlatformDef::getSurface(int width) {
			if (width < getMinWidth()) width = getMinWidth();
			
			SDL_Surface *dst = SDL_CreateRGBSurface(0, width, getHeight(), 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);

			int space = width;
			int leftSpacePadding = 0;

			auto left = _blocks["left"];
			if (left) {
				SDL_Rect rect{ 0, 0, left->w, left->h };
				SDL_BlitSurface(left, nullptr, dst, &rect);
				space -= left->w;
				leftSpacePadding = left->w;
			}

			auto right = _blocks["right"];
			if (right) {
				SDL_Rect rect{ dst->w - right->w, 0, right->w, right->h };
				SDL_BlitSurface(right, nullptr, dst, &rect);
				space -= right->w;
			}

			if (space > 0) {
				auto basic = _blocks["basic"];
				int numOfBasicBlocks = (int)ceil(space / basic->w);
				SDL_Rect rect{ 0, 0, basic->w, basic->h };
				for (int i = 0; i < numOfBasicBlocks; i++) {
					rect.x = leftSpacePadding + basic->w * i;
					SDL_BlitSurface(basic, nullptr, dst, &rect);
				}
			}			

			return dst;
		}

		int Editor::PlatformDef::calcWidth(int width) {
			if (width < getMinWidth()) 
				return getMinWidth();

			int total = 0;

			if (auto left = _blocks["left"]) total += left->w;
			if (auto right = _blocks["right"]) total += right->w;

			auto basic = _blocks["basic"];
			total += ceil((width - total) / basic->w) * basic->w;
			return total;
		}

		Editor::ToolbarButton::ToolbarButton(Editor &editor, std::string image) {
			SDL_Surface *surface = IMG_Load((editor.ICONS_FOLDER + image).c_str());
			_texture = SDL_CreateTextureFromSurface(editor._renderer, surface);
			SDL_FreeSurface(surface);
			_editor = &editor;
		}

		void Editor::ToolbarButton::draw(int x, int y) {
			if (isSelected()) {
				SDL_SetRenderDrawColor(_editor->_renderer, 240, 240, 240, 0);
				SDL_Rect toolbarRect{ x, y, 32, 32 };
				SDL_RenderFillRect(_editor->_renderer, &toolbarRect);
			}

			SDL_Rect textRect{ x + 4, y + 4, 24, 24 };
			SDL_RenderCopy(_editor->_renderer, _texture, nullptr, &textRect);
		}

		void Editor::save(std::string name) {
			JSON::JSONObject *rootObj = new JSON::JSONObject(nullptr);
			JSON::JSONDocument *doc = JSON::JSONDocument::fromRoot(*rootObj);

			rootObj->add("name", name);
			rootObj->add("editable", true);

			JSON::JSONArray *startingArr = new JSON::JSONArray(rootObj);

			int xPositions[] = { 150, 450, 750, 1050 };

			for (int i = 0; i < 4; i++){
				JSON::JSONObject *startObj = new JSON::JSONObject(startingArr);
				startObj->add("x", xPositions[i]);
				startObj->add("y", -100);
				startingArr->push(*startObj);
			}
			rootObj->add("startingPositions", *startingArr);

			JSON::JSONObject *gravityObj = new JSON::JSONObject(rootObj);
			gravityObj->add("left", 0);
			gravityObj->add("down", 100);
			rootObj->add("gravity", *gravityObj);

			JSON::JSONArray *platformArr = new JSON::JSONArray(rootObj);
			int imageIndex = 0;
			
			for (auto platform : _game->getWorld()->getPlatforms()) {
				std::string imageStr = "platform_" + std::to_string(imageIndex);
				SDL_Surface *surface;
				if (_drawnPlatformSurfaces.find(platform) != _drawnPlatformSurfaces.end()) {
					surface = _drawnPlatformSurfaces.at(platform);
				}
				else {
					surface = IMG_Load(_game->getEngine()->getDrawEngine()->getImagePath(platform).c_str());
				}
				IMG_SavePNG(surface, ("assets/levels/" + name + "/" + imageStr).c_str());

				JSON::JSONObject *levelObj = new JSON::JSONObject(platformArr);

				JSON::JSONObject *sizeObj = new JSON::JSONObject(levelObj);
				sizeObj->add("width", platform->getWidth());
				sizeObj->add("height", platform->getHeight());
				levelObj->add("size", *sizeObj);

				JSON::JSONObject *locationObj = new JSON::JSONObject(levelObj);
				locationObj->add("x", platform->getPixelX());
				locationObj->add("y", platform->getPixelY());
				levelObj->add("location", *locationObj);

				JSON::JSONObject *bodyPaddingObj = new JSON::JSONObject(levelObj);
				bodyPaddingObj->add("x", 0);
				bodyPaddingObj->add("y", 20);
				levelObj->add("bodyPadding", *bodyPaddingObj);

				levelObj->add("image", imageStr);

				platformArr->push(*levelObj);
				++imageIndex;
			}

			rootObj->add("platforms", *platformArr);

			doc->saveFile("assets/levels/" + name + "/data");
			delete doc;
						
			_game->getEngine()->getDrawEngine()->saveScreenshot("assets/levels/" + name + "/preview_big");
		}
	}
}