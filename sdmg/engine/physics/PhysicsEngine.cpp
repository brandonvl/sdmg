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
#include "model\MovablePlatform.h"
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
				_movablePlatforms = new std::vector < model::MovablePlatform*>();
				_movingGameObjects = new std::vector<MovableGameObject*>();
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
					checkMovablePlatforms();
				}
			}

			b2Body *PhysicsEngine::getBodyList()
			{
				return _world->GetBodyList();
			}

			void PhysicsEngine::checkMovablePlatforms()
			{
				//Mooi comments
				for (auto i = _movablePlatforms->begin(); i != _movablePlatforms->end(); i++)
				{
					// model::MovablePlatform *mp = static_cast<model::MovablePlatform*>((*i)->GetUserData());
					(*i)->checkDirectionChange();
				}

				for (int i = 0; i < _movingGameObjects->size(); i++)
				{
					MovableGameObject *gameObject = (*_movingGameObjects)[i];

					MovableGameObject::State state = gameObject->getState();

					switch (state)
					{
					case  MovableGameObject::State::IDLE:
						gameObject->getBody()->SetLinearVelocity(b2Vec2(0.0f, gameObject->getBody()->GetLinearVelocity().y));
						_movingGameObjects->erase(_movingGameObjects->begin() + i);
						i--;
						break;
					case MovableGameObject::State::WALKING:

						if (gameObject->getDirection() == MovableGameObject::Direction::LEFT)
							doAction(gameObject, PhysicsEngine::Action::MOVELEFT);
						else if (gameObject->getDirection() == MovableGameObject::Direction::RIGHT)
							doAction(gameObject, PhysicsEngine::Action::MOVERIGHT);
						break;
					}
				}
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

			b2Body *PhysicsEngine::addBody(GameObject *object)
			{
				return addBody(object, false, 0, 0);
			}

			b2Body *PhysicsEngine::addBody(MovableGameObject *object)
			{
				return addBody(object, true, 0, 0);
			}

			b2Body *PhysicsEngine::addBody(GameObject *object, float paddingX, float paddingY)
			{
				return addBody(object, false, paddingX, paddingY);
			}

			b2Body *PhysicsEngine::addBody(MovableGameObject *object, float paddingX, float paddingY)
			{
				return addBody(object, true, paddingX, paddingY);
			}

			b2Body *PhysicsEngine::addBody(int x, int y, int w, int h, bool dyn, GameObject *object)
			{
				b2BodyDef *bodydef = new b2BodyDef();
				bodydef->position.Set(x * _P2M, y * _P2M);
				if (dyn)
					bodydef->type = b2_dynamicBody;
				b2Body *body = _world->CreateBody(bodydef);

				body->SetFixedRotation(true);
				// bodydef->userData = &body;

				b2PolygonShape *shape = new b2PolygonShape();
				shape->SetAsBox(_P2M * w / 2, _P2M * h / 2);

				b2FixtureDef *fixturedef = new b2FixtureDef();
				fixturedef->shape = shape;
				fixturedef->density = 1.0f;
				body->CreateFixture(fixturedef);

				body->SetUserData(object);
				object->setBody(body);

				object->setLocation(&body->GetPosition().x, &body->GetPosition().y);

				//  b2Vec2 *vec = new b2Vec2(P2M*w, P2M*h);
				//  _boxSizes->insert(std::pair<b2Body*, b2Vec2*>(body, vec));

				return body;
			}

			b2Body *PhysicsEngine::addBody(GameObject *object, bool dynamic, float paddingX, float paddingY)
			{
				b2BodyDef *bodydef = new b2BodyDef();
				bodydef->position.Set(object->getX() * _P2M, object->getY() * _P2M);
				if (dynamic)
					bodydef->type = b2_dynamicBody;
				b2Body *body = _world->CreateBody(bodydef);

				body->SetFixedRotation(true);
				// bodydef->userData = &body;

				b2PolygonShape *shape = new b2PolygonShape();

				shape->SetAsBox(_P2M * (object->getWidth() - paddingX) / 2, _P2M * (object->getHeight() - paddingY) / 2);

				b2FixtureDef *fixturedef = new b2FixtureDef();
				fixturedef->shape = shape;
				fixturedef->density = 1.0f;
				body->CreateFixture(fixturedef);

				body->SetUserData(object);
				object->setBody(body);
				
				object->setLocation(&body->GetPosition().x, &body->GetPosition().y);

				//  b2Vec2 *vec = new b2Vec2(P2M*w, P2M*h);
				//  _boxSizes->insert(std::pair<b2Body*, b2Vec2*>(body, vec));

				return body;
			}

			b2Body* PhysicsEngine::addKinematicBody(model::MovablePlatform *object)
			{
				b2BodyDef *bodydef = new b2BodyDef();
				bodydef->position.Set(object->getStartLocationX() *_P2M, object->getStartLocationY() *_P2M);
				bodydef->type = b2_kinematicBody;
				b2Body* body = _world->CreateBody(bodydef);

				body->SetFixedRotation(true);

				b2PolygonShape *shape = new b2PolygonShape();
				shape->SetAsBox(_P2M*object->getWidth() / 2, _P2M*object->getHeight() / 2);

				b2FixtureDef *fixturedef = new b2FixtureDef();
				fixturedef->shape = shape;
				fixturedef->density = 1.0f;
				body->CreateFixture(fixturedef);

				if (object->getDirection() == MovableGameObject::Direction::RIGHT || object->getDirection() == MovableGameObject::Direction::DOWN)
					body->SetLinearVelocity(b2Vec2(object->getSpeed().horizontal, object->getSpeed().vertical));
				else if (object->getDirection() == MovableGameObject::Direction::LEFT || object->getDirection() == MovableGameObject::Direction::UP)
					body->SetLinearVelocity(b2Vec2(-object->getSpeed().horizontal, -object->getSpeed().vertical));

				body->SetUserData(object);
				object->setBody(body);


				// object->setLocation(b2Vec2(object->getStartLocationX() *_P2M, object->getStartLocationY() *_P2M));
				object->setLocation(&body->GetPosition().x, &body->GetPosition().y);

				_movablePlatforms->push_back(object);

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

				if (direction == KinematicBody::Direction::RIGHT)
				{
					body->SetLinearVelocity(b2Vec2(speed, 0.0));
					kinematicBody->setDirection(KinematicBody::Direction::RIGHT);
					kinematicBody->setEndLocation(new b2Vec2(endpoint*_P2M, y*_P2M));
				}
				else
				{
					body->SetLinearVelocity(b2Vec2(0.0, speed));
					kinematicBody->setDirection(KinematicBody::Direction::DOWN);
					kinematicBody->setEndLocation(new b2Vec2(x*_P2M, endpoint*_P2M));
				}

				KinematicBody::Direction dir = kinematicBody->GetDirection();

				body->SetUserData(kinematicBody);

				return body;
			}

			void PhysicsEngine::registerAction(MovableGameObject *object)
			{
				_movingGameObjects->push_back(object);
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
