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
				_step = 1.0f / 60.0f;
				_lastUpdate = std::chrono::high_resolution_clock::now();

				initializeActions();
			}

			PhysicsEngine::~PhysicsEngine() 
			{
				cleanUp();
				delete _world;
				delete _actionHandler;
				delete _contactListener;
				delete _contactFilter;
			}

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
					checkMovableBodies();
				}
			}

			void PhysicsEngine::pause() {
				_enabled = false;
			}

			void PhysicsEngine::resume() {
				_enabled = true;
				_lastUpdate = std::chrono::high_resolution_clock::now();
			}


			void PhysicsEngine::cleanUp()
			{
				if (!_world->IsLocked())
				{
					b2Body* body = nullptr;
					b2Body* nextBody = _world->GetBodyList();

					while (nextBody)
					{
						body = nextBody;
						nextBody = nextBody->GetNext();

						_world->DestroyBody(body);
					}
				}
			}

			b2Body *PhysicsEngine::getBodyList()
			{
				return _world->GetBodyList();
			}

			void PhysicsEngine::checkMovableBodies()
			{
				b2Body *body = _world->GetBodyList();
				//  int count = 0;

				while (body)
				{

					if (body->GetType() == b2_kinematicBody)
					{
						model::MovablePlatform *kinematicBody = static_cast<model::MovablePlatform*>(body->GetUserData());
						kinematicBody->checkDirectionChange();
					}
					else if (body->GetType() == b2_dynamicBody)
					{
						MovableGameObject *gameObject = static_cast<MovableGameObject*>(body->GetUserData());
						MovableGameObject::State state = gameObject->getState();

						//  float y = body->GetPosition().y * 20.0f;
						if (body->GetPosition().y * 20.0f > 1000.0f || state == MovableGameObject::State::RESPAWN)
						{
							doAction(gameObject, PhysicsEngine::Action::RESPAWN);
						}
						else
						{
							switch (state)
							{
							case  MovableGameObject::State::IDLE:
								if (body->GetLinearVelocity().x != 0.0f)
									doAction(gameObject, PhysicsEngine::Action::IDLE);
								break;
							case MovableGameObject::State::WALKING:
								if (gameObject->getDirection() == MovableGameObject::Direction::LEFT)
									doAction(gameObject, PhysicsEngine::Action::MOVELEFT);
								else if (gameObject->getDirection() == MovableGameObject::Direction::RIGHT)
									doAction(gameObject, PhysicsEngine::Action::MOVERIGHT);
								break;
							case MovableGameObject::State::RESPAWN:
								doAction(gameObject, PhysicsEngine::Action::RESPAWN);
								break;
							case MovableGameObject::State::KNEELING:
								doAction(gameObject, PhysicsEngine::Action::KNEEL);
								break;


							case MovableGameObject::State::MIDRANGEATTACKBEGIN:
								doAction(gameObject, PhysicsEngine::Action::MIDRANGEATTACKBEGIN);
								break;
							case MovableGameObject::State::MIDRANGEATTACK:
								doAction(gameObject, PhysicsEngine::Action::MIDRANGEATTACK);
								break;
							case MovableGameObject::State::MIDRANGEATTACKEND:
								doAction(gameObject, PhysicsEngine::Action::MIDRANGEATTACKEND);
								break;




							case MovableGameObject::State::KNOCKBACKLEFT:
								doAction(gameObject, PhysicsEngine::Action::KNOCKBACKLEFT);
								if (gameObject->getAttackBody() != nullptr)
									doAction(gameObject, PhysicsEngine::Action::MIDRANGEATTACKEND);
								break;
							case MovableGameObject::State::KNOCKBACKRIGHT:
								doAction(gameObject, PhysicsEngine::Action::KNOCKBACKRIGHT);
								if (gameObject->getAttackBody() != nullptr)
									doAction(gameObject, PhysicsEngine::Action::MIDRANGEATTACKEND);
								break;
							case MovableGameObject::State::JUMPING:
								if (body->GetLinearVelocity().y >= -0.2f)
									gameObject->setState(MovableGameObject::State::FALLING);
								body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));
								break;
							case MovableGameObject::State::JUMPINGLEFT:
								if (body->GetLinearVelocity().y >= -0.2f)
									gameObject->setState(MovableGameObject::State::FALLINGLEFT);
								if (gameObject->getDirection() == MovableGameObject::Direction::LEFT)
									doAction(gameObject, PhysicsEngine::Action::MOVELEFT);
							case MovableGameObject::State::JUMPINGRIGHT:
								if (body->GetLinearVelocity().y >= -0.2f)
									gameObject->setState(MovableGameObject::State::FALLINGRIGHT);
								if (gameObject->getDirection() == MovableGameObject::Direction::RIGHT)
									doAction(gameObject, PhysicsEngine::Action::MOVERIGHT);
								break;



							case MovableGameObject::State::FALLING:
								body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));
								break;
							case MovableGameObject::State::FALLINGLEFT:
								if (gameObject->getDirection() == MovableGameObject::Direction::LEFT)
									doAction(gameObject, PhysicsEngine::Action::MOVELEFT);
							case MovableGameObject::State::FALLINGRIGHT:
								if (gameObject->getDirection() == MovableGameObject::Direction::RIGHT)
									doAction(gameObject, PhysicsEngine::Action::MOVERIGHT);
								break;

							}
						}
					}
					body = body->GetNext();
				}
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
				//  fixturedef->friction = 0.0f;
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

				//  bodydef->position.Set(object->getStartLocationX() * _P2M, object->getStartLocationY() * _P2M);
				bodydef->position.Set(object->getX() * _P2M, object->getY() * _P2M);
				
				bodydef->type = b2_kinematicBody;
				b2Body* body = _world->CreateBody(bodydef);

				body->SetFixedRotation(true);

				b2PolygonShape *shape = new b2PolygonShape();
				shape->SetAsBox((_P2M*object->getWidth()) / 2, (_P2M*object->getHeight()) / 2);

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

				float32 x = body->GetPosition().x;
				float32 y = body->GetPosition().y;
				// object->setLocation(b2Vec2(object->getStartLocationX() *_P2M, object->getStartLocationY() *_P2M));
				object->setLocation(body->GetPosition().x, body->GetPosition().y);
				object->setLocation(&body->GetPosition().x, &body->GetPosition().y);

				float32 as = object->getPixelX();
				//  object->setLocation(object->getStartLocationX() * _P2M, object->getStartLocationY() * _P2M);

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
				addAction(Action::KNEEL, &PhysicsEngineActionHandler::kneel);
				addAction(Action::SHORTRANGEATTACK, &PhysicsEngineActionHandler::shortRangeAttack);
				addAction(Action::LONGRANGEATTACK, &PhysicsEngineActionHandler::longRangeAttack);
				addAction(Action::RESPAWN, &PhysicsEngineActionHandler::respawn);
				addAction(Action::KNOCKBACKLEFT, &PhysicsEngineActionHandler::knockbackLeft);
				addAction(Action::KNOCKBACKRIGHT, &PhysicsEngineActionHandler::knockbackRight);



				addAction(Action::MIDRANGEATTACKBEGIN, &PhysicsEngineActionHandler::midRangeAttackBegin);
				addAction(Action::MIDRANGEATTACK, &PhysicsEngineActionHandler::midRangeAttack);
				addAction(Action::MIDRANGEATTACKEND, &PhysicsEngineActionHandler::midRangeAttackEnd);
			}
		}
	}
}
