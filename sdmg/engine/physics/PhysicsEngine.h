//
//
//  @ Project : SDMG
//  @ File Name : PhysicsEngine.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include <map>

class b2World;
class b2Body;

namespace sdmg {
	namespace engine {
		class Engine;
		class GameObject;
		class MovableGameObject;

		namespace physics {
			class PhysicsEngineActionHandler;

			class PhysicsEngine {
			public:
				PhysicsEngine();
				virtual ~PhysicsEngine();
				void update();
				void pause();
				void resume();
				void moveBody();
				void setWorldGravity(const float leftGravity, const float downGravity);
				b2Body *addBody(int x, int y, int w, int h, bool dyn, GameObject *object);
				enum class Action { MOVELEFT, MOVERIGHT, IDLE, JUMP, SHORTATTACK, MIDDLEATTACK, LONGATTACK };
				void doAction(MovableGameObject *object, Action action);
			private:
				Engine *_engine;
				b2World *_world;
				bool _enabled;
				const float _M2P = 20.0f;
				const float _P2M = 1.0f / _M2P;
				PhysicsEngineActionHandler *_actionHandler;


				typedef void(PhysicsEngineActionHandler::*ActionFunction)(MovableGameObject*);
				void addAction(Action action, ActionFunction function);
				typedef std::map<Action, ActionFunction> ActionMap;
				ActionMap _actionMap;
				void initializeActions();
			};
		}
	}
}