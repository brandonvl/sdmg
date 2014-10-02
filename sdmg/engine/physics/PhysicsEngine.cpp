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
#include "engine\physics\ContactListener.h"
#include "engine\physics\KinematicBody.h"
#include "engine\GameTime.h"
#include "Box2D\Box2D.h"
#include "PhysicsEngineActionHandler.h"

namespace sdmg {
	namespace engine {
		namespace physics {

			PhysicsEngine::PhysicsEngine()
			{
				_world = new b2World(b2Vec2(0.0f, 0.0f));
				_actionHandler = new PhysicsEngineActionHandler();
				_contactListener = new ContactListener();
				_contactFilter = new b2ContactFilter();
				_world->SetContactListener(_contactListener);
				_world->SetContactFilter(_contactFilter);
				_step = 1.0f / 60.0f;
				_lastUpdate = std::chrono::high_resolution_clock::now();

				initializeActions();
			}

			PhysicsEngine::~PhysicsEngine() { }

			void PhysicsEngine::update() {
				if (_enabled)
				{
					auto curTime = std::chrono::high_resolution_clock::now();
					float diff = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - _lastUpdate).count() / 1000.0f;

					_lastUpdate = curTime;
					_accumulator += diff;

					while (_accumulator > _step) {
						_world->Step(_step, 8, 3);
						_accumulator -= _step;
					}
				}
			}

			b2Body *PhysicsEngine::getBodyList()
			{
				return _world->GetBodyList();
			}

			void checkKinematicBodies()
			{
				/*
				for (auto i = _kinematicBodies->begin(); i != _kinematicBodies->end(); i++)
				{
					KinematicBody kinematicBody = static_cast<KinematicBody*>((*i)->GetUserData());
					kinematicBody->checkDirectionChange();
				}
				*/
			}
			
			void PhysicsEngine::pause() {
				_enabled = false;
			}
			
			void PhysicsEngine::resume() {
				_enabled = true;
			}

			b2Vec2 PhysicsEngine::getWorldGravity()
			{
				return _world->GetGravity();
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
				// bodydef->userData = &body;

				b2PolygonShape *shape = new b2PolygonShape();
				shape->SetAsBox(_P2M*w / 2, _P2M*h / 2);

				b2FixtureDef *fixturedef = new b2FixtureDef();
				fixturedef->shape = shape;
				fixturedef->density = 1.0f;
				body->CreateFixture(fixturedef);

				body->SetUserData(object);
				object->setBody(body);

				const float32 *objectx;
				objectx = &body->GetPosition().x;

				object->setLocation(&body->GetPosition().x, &body->GetPosition().y);

				//  b2Vec2 *vec = new b2Vec2(P2M*w, P2M*h);
				//  _boxSizes->insert(std::pair<b2Body*, b2Vec2*>(body, vec));

				return body;
			}

			b2Body* PhysicsEngine::addKinematicBody(int x, int y, int w, int h, int speed, int endpoint, KinematicBody::Direction direction)
			{
				b2BodyDef *bodydef = new b2BodyDef();
				bodydef->position.Set(x*_P2M, y*_P2M);
				bodydef->type = b2_kinematicBody;
				b2Body* body = _world->CreateBody(bodydef);

				body->SetFixedRotation(true);

				b2PolygonShape *shape = new b2PolygonShape();
				shape->SetAsBox(_P2M*w / 2, _P2M*h / 2);

				b2FixtureDef *fixturedef = new b2FixtureDef();
				fixturedef->shape = shape;
				fixturedef->density = 1.0f;
				body->CreateFixture(fixturedef);

				b2Vec2 *vec = new b2Vec2(_P2M*w, _P2M*h);
				//  _boxSizes->insert(std::pair<b2Body*, b2Vec2*>(body, vec));

				KinematicBody *kinematicBody = new KinematicBody(new b2Vec2(x*_P2M, y*_P2M));

				if (direction == KinematicBody::Direction::Right)
				{
					body->SetLinearVelocity(b2Vec2(speed, 0.0));
					kinematicBody->setDirection(KinematicBody::Direction::Right);
					kinematicBody->setEndLocation(new b2Vec2(endpoint*_P2M, y*_P2M));
				}
				else
				{
					body->SetLinearVelocity(b2Vec2(0.0, speed));
					kinematicBody->setDirection(KinematicBody::Direction::Down);
					kinematicBody->setEndLocation(new b2Vec2(x*_P2M, endpoint*_P2M));
				}

				KinematicBody::Direction dir = kinematicBody->GetDirection();

				body->SetUserData(kinematicBody);

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
