//
//
//  @ Project : SDMG
//  @ File Name : PhysicsEngine.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "PhysicsEngine.h"
#include "engine\Engine.h"
#include "engine\GameBase.h"
#include "engine\World.h"
#include "engine\GameObject.h"
#include "engine\MovableGameObject.h"
#include "Box2D\Box2D.h"
#include "PhysicsEngineActionHandler.h"

namespace sdmg {
	namespace engine {
		namespace physics {

			PhysicsEngine::PhysicsEngine()
			{
				_world = new b2World(b2Vec2(0.0f, 0.0f));
				_actionHandler = new PhysicsEngineActionHandler();
				initializeActions();
			}

			PhysicsEngine::~PhysicsEngine() { }

			void PhysicsEngine::update() {
				if (_enabled)
					_world->Step(1 / 60, 8, 3);
			}
			
			void PhysicsEngine::pause() {
				_enabled = false;
			}
			
			void PhysicsEngine::resume() {
				_enabled = true;
			}

			void PhysicsEngine::setWorldGravity(const float leftGravity, const float downGravity)
			{
				_world->SetGravity(b2Vec2(leftGravity, downGravity));
			}

			b2Body *PhysicsEngine::addBody(int x, int y, int w, int h, bool dyn, GameObject *object)
			{
				b2BodyDef *bodydef = new b2BodyDef();
				bodydef->position.Set(x*_P2M, y*_P2M);
				if (dyn)
					bodydef->type = b2_dynamicBody;
				b2Body *body = _world->CreateBody(bodydef);

				body->SetFixedRotation(true);
				bodydef->userData = &body;

				b2PolygonShape *shape = new b2PolygonShape();
				shape->SetAsBox(_P2M*w / 2, _P2M*h / 2);

				b2FixtureDef *fixturedef = new b2FixtureDef();
				fixturedef->shape = shape;
				fixturedef->density = 1.0f;
				body->CreateFixture(fixturedef);

				body->SetUserData(object);

				const float32 *objectx;
				objectx = &body->GetPosition().x;

				//  b2Vec2 *vec = new b2Vec2(P2M*w, P2M*h);
				//  _boxSizes->insert(std::pair<b2Body*, b2Vec2*>(body, vec));

				return body;
			}

			void PhysicsEngine::doAction(MovableGameObject *object, PhysicsEngine::Action action)
			{
				(_actionHandler->*(_actionMap[action]))(object);
			}

			void PhysicsEngine::addAction(Action action, ActionFunction function) {
				_actionMap.insert(std::make_pair(action, function));
			}

			void PhysicsEngine::initializeActions() {
				addAction(Action::MOVELEFT, &PhysicsEngineActionHandler::moveLeft);
				addAction(Action::MOVERIGHT, &PhysicsEngineActionHandler::moveRight);
				addAction(Action::JUMP, &PhysicsEngineActionHandler::jump);
				addAction(Action::IDLE, &PhysicsEngineActionHandler::idle);
				addAction(Action::SHORTATTACK, &PhysicsEngineActionHandler::shortAttack);
				addAction(Action::MIDDLEATTACK, &PhysicsEngineActionHandler::middleAttack);
				addAction(Action::LONGATTACK, &PhysicsEngineActionHandler::longAttack);
			}
		}
	}
}
