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
			
			loadPlatformDefs();
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
		}
		
		void Editor::loadPlatformDefs() {

			std::vector<std::string> folders = engine::util::FileManager::getInstance().getFolders(PLATFORM_FOLDER);

			for (auto folder : folders) {
				_platformDefs.push_back(new PlatformDef(*this, folder));				
			}

		}

		void Editor::handleEvent(SDL_Event &event) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_F4)
					disable();
			}
			else if (event.window.windowID == SDL_GetWindowID(_window)) {
				if (event.type == SDL_MOUSEBUTTONDOWN) {
					int x, y;
					SDL_GetMouseState(&x, &y);

					int i = floor(y / 40);
					if (i < _platformDefs.size()) {
						_currentPlatformDef = _platformDefs[i];
					}
					else {
						_currentPlatformDef = nullptr;
					}

					_mouseDownX = -1;
				}
			}
			else if (_currentPlatformDef == nullptr) {
				_game->getEngine()->getInputEngine()->handleEvent(event);
			}
			else {
				if (event.type == SDL_MOUSEBUTTONDOWN) {
					int x, y;
					SDL_GetMouseState(&x, &y);
					_mouseDownX = x;
					printf("Mouse down\n");
				}
				else if (event.type == SDL_MOUSEBUTTONUP){
					createPlatform();
					_mouseDownX = -1;
					printf("Mouse up\n");
				}
			}
		}

		void Editor::createHitbox(GameObject *obj) {
			_hitboxes->insert(std::make_pair(obj, _game->getEngine()->getInputEngine()->getMouse().setClickAction(obj->getX() * 20.0f - obj->getWidth() / 2, obj->getY() * 20.0f - obj->getHeight() / 2, obj->getWidth(), obj->getHeight(), (std::function<void()>)[&, obj] { selectObject(*obj); })));
		}

		void Editor::createPlatform(){
			int x, y, w, h = _currentPlatformDef->getHeight();
			SDL_GetMouseState(&x, &y);
			
			w = _currentPlatformDef->calcWidth(x - _mouseDownX);
			int posX = _mouseDownX + w / 2;
			
			model::Platform *platform = new model::Platform(false);
			platform->setSize(w, h);
			platform->setLocation(posX, y);

			int bodyPaddingX = 30, bodyPaddingY = 20;

			_game->getEngine()->getPhysicsEngine()->addBody(platform, bodyPaddingX, bodyPaddingY);
			_game->getWorld()->addPlatform(platform);

			SDL_Surface *surface = _currentPlatformDef->getSurface(w);
			_game->getEngine()->getDrawEngine()->load(platform, surface);

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

				SDL_RenderPresent(_renderer);
			}
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
			if (_curSelectedObject) {
				_curSelectedObject->getBody()->SetTransform(b2Vec2((x - _mouseDownX) / 20.f, (y - _mouseDownY) / 20.0f), _curSelectedObject->getBody()->GetAngle());

				if (_hitboxes->count(_curSelectedObject)) {
					input::Mouse::Hitbox *hitbox = _hitboxes->at(_curSelectedObject);
					hitbox->x = _curSelectedObject->getX() * 20.0f - _curSelectedObject->getWidth() / 2;
					hitbox->y = _curSelectedObject->getY() * 20.0f - _curSelectedObject->getHeight() / 2;
				}
			}
		}

		void Editor::selectObject(GameObject &gameObject) {
			_mouseDownX = _game->getEngine()->getInputEngine()->getMouse().getX() - gameObject.getX() * 20.0f;
			_mouseDownY = _game->getEngine()->getInputEngine()->getMouse().getY() - gameObject.getY() * 20.0f;
			_curSelectedObject = &gameObject;
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
				int numOfBasicBlocks = ceil(space / basic->w);
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
	}
}