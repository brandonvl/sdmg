#include "Editor.h"
#include "engine\GameBase.h"
#include "engine\Engine.h"
#include "engine\physics\PhysicsEngine.h"
#include "engine\drawing\DrawEngine.h"
#include "engine\input\InputEngine.h"
#include "engine\input\Mouse.h"
#include "engine\World.h"
#include "engine\util\FileManager.h"

namespace sdmg {
	namespace helperclasses {
		

		Editor::Editor(GameBase &game)
		{
			_game = &game;
			_hitboxes = new std::map<GameObject*, input::Mouse::Hitbox*>();

			_game->getEngine()->getDrawEngine()->loadText("editmode", "Edit mode", { 255, 255, 255 }, "arial", 18);
		}


		Editor::~Editor()
		{
			delete _hitboxes;
			_hitboxes = nullptr;

			_game->getEngine()->getDrawEngine()->unloadText("editmode");

			for (auto def : _platformDefs) {
				delete def;
			}
			_platformDefs.clear();
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
				_hitboxes->insert(std::make_pair(obj, inputEngine->getMouse().setClickAction(obj->getX() * 20.0f - obj->getWidth() / 2, obj->getY() * 20.0f - obj->getHeight() / 2, obj->getWidth(), obj->getHeight(), (std::function<void()>)[&, obj] { selectObject(*obj); })));
			}

			inputEngine->getMouse().setMouseMoveAction((std::function<void(int x, int y)>)[&](int x, int y) { mouseMove(x, y); });
			inputEngine->getMouse().setMouseUpAction((std::function<void()>)[&] { _curSelectedObject = nullptr; });
			inputEngine->setMouseEnabled(true);

			_window = SDL_CreateWindow("Elements", 10, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
			_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
		}

		void Editor::disable() {
			_enabled = false;
			_hitboxes->clear();

			_game->getEngine()->getInputEngine()->getMouse().clear();
			_game->getEngine()->getInputEngine()->setMouseEnabled(false);
			_game->getEngine()->getPhysicsEngine()->resume();

			SDL_DestroyRenderer(_renderer);
			SDL_DestroyWindow(_window);
		}
		
		void Editor::loadPlatformDefs() {

			std::vector<std::string> folders = engine::util::FileManager::getInstance().getFolders(PLATFORM_FOLDER);

			for (auto folder : folders) {
				_platformDefs.push_back(new PlatformDef(folder));				
			}

		}

		void Editor::update() {

		}

		void Editor::draw() {
			if (_enabled) {
				_game->getEngine()->getDrawEngine()->drawRectangle(Rectangle(0, 0, 1920, 40), 0, 0, 0);
				_game->getEngine()->getDrawEngine()->drawText("editmode", 10, 10);

				SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 0);
				SDL_RenderClear(_renderer);
				SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 0);

				drawPlatforms();			

				SDL_RenderPresent(_renderer);
			}
		}

		void Editor::drawPlatforms() {
			for (auto def : _platformDefs) {
				SDL_Rect rect;
				rect.h = 50;
				rect.w = 200;
				rect.x = 0;
				rect.y = 0;

				SDL_RenderFillRect(_renderer, &rect);
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
	}
}