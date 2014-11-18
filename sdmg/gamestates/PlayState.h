//
//
//  @ Project : SDMG
//  @ File Name : PlayState.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "engine\GameState.h"
#include "engine\GameObject.h"
#include "engine\input\Mouse.h"
#include <vector>
#include <map>

using namespace sdmg::engine;

namespace sdmg {
	namespace model {
		class Platform;
		class Character;
		class MovablePlatform;
	}
	namespace helperclasses{
		class HUD;
	}
	namespace gamestates {
		class PlayState : public GameState {
		public:
			virtual void init(GameBase &game);
			virtual void cleanup(GameBase &game);

			virtual void pause(GameBase &game);
			virtual void resume(GameBase &game);

			virtual void handleEvents(GameBase &game, GameTime &gameTime);
			virtual void update(GameBase &game, GameTime &gameTime);
			virtual void draw(GameBase &game, GameTime &gameTime);
			static PlayState& getInstance() {
				static PlayState _instance;
				return _instance;
			}

			void setCharacters(std::vector<model::Character*> *characters);
			void setPlatform(model::Platform *platform);
			void setBullets(std::vector<model::MovablePlatform *> *bullets);
			void setHUDs(std::vector<helperclasses::HUD *> *huds);

		protected:
			PlayState() { }
			void preformDraw(GameBase &game);
			std::vector<helperclasses::HUD*> *_huds;

			bool _showFPS, _showHitBoxes, _editMode, _showClickBoxes;
			long _fps;

			friend class GameOverState;

			void enableEditMode(GameBase &game);
			void disableEditMode(GameBase &game);
			void mouseMove(int x, int y);
			void selectObject(GameObject &gameObject);

			float _mouseDownX, _mouseDownY;
			GameObject *_curSelectedObject;
			GameBase *_game;

			std::map<GameObject*, input::Mouse::Hitbox*> *_hitboxes;
		};
	}
}