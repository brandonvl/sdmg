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
#include <vector>
#include "engine\physics\KinematicBody.h"
#include <chrono>

class b2World;
class b2Body;

namespace sdmg {
	namespace model
	{
		class MovablePlatform;
	}
	namespace engine {
		class Engine;
		class GameObject;
		class MovableGameObject;
		class GameTime;

		namespace physics {
			class PhysicsEngineActionHandler;
			class ContactListener;

			class PhysicsEngine {
			public:
				PhysicsEngine(Engine *engine);
				virtual ~PhysicsEngine();
				void update();
				void pause();
				void resume();
				void cleanUp();
				void moveBody();
				void resetBobs();
				b2Body *getBodyList();
				void setWorldGravity(const float leftGravity, const float downGravity);
				b2Vec2 getWorldGravity();
				b2Body *addBody(GameObject *object);
				b2Body *addBody(MovableGameObject *object);
				b2Body *addBody(GameObject *object, float paddingX, float paddingY);
				b2Body *addBody(MovableGameObject *object, float paddingX, float paddingY);
				b2Body* addBody(int x, int y, int w, int h, bool dyn, GameObject *object);
				b2Body* addKinematicBody(model::MovablePlatform *object);
				b2Body* addKinematicBody(int x, int y, int w, int h, int speed, int endpoint, KinematicBody::Direction direction);
				enum class Action { MOVELEFT, MOVERIGHT, IDLE, JUMP, KNEEL, RESPAWN, KNOCKBACKLEFT, KNOCKBACKRIGHT,
					MIDRANGEATTACKBEGIN, MIDRANGEATTACK, MIDRANGEATTACKEND, LONGRANGEATTACKBEGIN, LONGRANGEATTACK, LONGRANGEATTACKEND
				};
				void doAction(MovableGameObject *object, Action action);
			private:
				Engine *_engine;
				b2World *_world;
				bool _enabled;
				const float _M2P = 20.0f;
				const float _P2M = 1.0f / _M2P;

				ContactListener *_contactListener;
				b2ContactFilter *_contactFilter;
				PhysicsEngineActionHandler *_actionHandler;

				b2Body *addBody(GameObject *object, bool dynamic, float paddingX, float paddingY);
				void checkMovableBodies();

				std::chrono::high_resolution_clock::time_point _lastUpdate;
				float _step, _accumulator;

				typedef void(PhysicsEngineActionHandler::*ActionFunction)(MovableGameObject*);
				void addAction(Action action, ActionFunction function);
				typedef std::map<Action, ActionFunction> ActionMap;
				ActionMap _actionMap;
				void initializeActions();
			};
		}
	}
}