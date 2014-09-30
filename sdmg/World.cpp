#include "World.h"
#include "Box2D\Box2D.h"
#include "sdl\include\SDL.h"
#include "KinecticBody.h"
#include <iostream>
#include <map>
#include <cmath>  
#include <vector>


b2World *_world;
float M2P, P2M;
const int WIDTH = 1280, HEIGHT = 720;
std::map<b2Body*, b2Vec2*> *_boxSizes;
std::map<b2Body*, KinecticBody*> *_kinecticBodies;

class MyContactListener : public b2ContactListener
{
	virtual void BeginContact(b2Contact* contact) {
		std::cout << "I'm Hit";

		if (contact->GetFixtureA()->GetBody()->GetType() == b2_kinematicBody)
		{
			contact->GetFixtureB()->GetBody()->SetLinearVelocity(contact->GetFixtureA()->GetBody()->GetLinearVelocity());
 			KinecticBody *kinecticBody = static_cast<KinecticBody*>(contact->GetFixtureA()->GetBody()->GetUserData()); // (*_kinecticBodies)[tmp];
			kinecticBody->addBody(contact->GetFixtureB()->GetBody(), contact->GetFixtureA()->GetBody()->GetLinearVelocity());
		}

		std::cout << contact->GetFixtureA() << " botst tegen " << contact->GetFixtureB() << std::endl;


	}

	virtual void EndContact(b2Contact* contact) {

		if (contact->GetFixtureA()->GetBody()->GetType() == b2_kinematicBody)
		{
			KinecticBody *kinecticBody = static_cast<KinecticBody*>(contact->GetFixtureA()->GetBody()->GetUserData()); // (*_kinecticBodies)[tmp];
			kinecticBody->removeBody(contact->GetFixtureB()->GetBody());
		}
	}

	virtual void PreSolve(b2Contact* contact, const b2Manifold *oldManifold) {
		//  std::cout << "PreSolve" << std::endl;
	}

	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse *impulse) {
		//  std::cout << "Postsolve" << std::endl;
	}

};

MyContactListener *listener;
b2ContactFilter *filter;

World::World(b2Vec2 gravtiy)
{
	M2P = 20.0f;
	P2M = 1.0f / M2P;

	_world = new b2World(gravtiy);
	//_world = new b2World(b2Vec2(0.0, 9.81));

	filter = new b2ContactFilter();
	listener = new MyContactListener();
	_world->SetContactListener(listener);
	_world->SetContactFilter(filter);

	_boxSizes = new std::map < b2Body*, b2Vec2* > ;
	_kinecticBodies = new std::map < b2Body*, KinecticBody* > ;
}

World::~World()
{
}

void World::setGravity(b2Vec2 gravtiy)
{
	_world->SetGravity(gravtiy);
}

b2Vec2 World::getGravity()
{
	return _world->GetGravity();
}

void World::step(float32 timeStep, int32 velocityIterations, int32 positionIterators)
{
	//_world->step(timeStep, 8, 3);
	_world->Step(timeStep, velocityIterations, positionIterators);
}

b2Body* World::addBody(int x, int y, int w, int h, bool dyn)
{
	b2BodyDef *bodydef = new b2BodyDef();
	bodydef->position.Set(x*P2M, y*P2M);
	if (dyn)
		bodydef->type = b2_dynamicBody;
	b2Body* body = _world->CreateBody(bodydef);

	body->SetFixedRotation(true);
	//bodydef->userData = &body;

	b2PolygonShape *shape = new b2PolygonShape();
	shape->SetAsBox(P2M*w / 2, P2M*h / 2);

	b2FixtureDef *fixturedef = new b2FixtureDef();
	fixturedef->shape = shape;
	fixturedef->density = 1.0f;
	body->CreateFixture(fixturedef);

	b2Vec2 *vec = new b2Vec2(P2M*w, P2M*h);
	_boxSizes->insert(std::pair<b2Body*, b2Vec2*>(body, vec));

	return body;
}

b2Body* World::addKinecticBody(int x, int y, int w, int h, int speed, int endpoint, KinecticBody::Direction direction)
{
	b2BodyDef *bodydef = new b2BodyDef();
	bodydef->position.Set(x*P2M, y*P2M);
	bodydef->type = b2_kinematicBody;
	b2Body* body = _world->CreateBody(bodydef);

	body->SetFixedRotation(true);
	//bodydef->userData = &body;

	b2PolygonShape *shape = new b2PolygonShape();
	shape->SetAsBox(P2M*w / 2, P2M*h / 2);

	b2FixtureDef *fixturedef = new b2FixtureDef();
	fixturedef->shape = shape;
	fixturedef->density = 1.0f;
	body->CreateFixture(fixturedef);

	b2Vec2 *vec = new b2Vec2(P2M*w, P2M*h);
	_boxSizes->insert(std::pair<b2Body*, b2Vec2*>(body, vec));

	KinecticBody *kinecticBody = new KinecticBody(new b2Vec2(x*P2M, y*P2M));

	if (direction == KinecticBody::Direction::Right)
	{
		body->SetLinearVelocity(b2Vec2(speed, 0.0));
		kinecticBody->setDirection(KinecticBody::Direction::Right);
		kinecticBody->setEndLocation(new b2Vec2(endpoint*P2M, y*P2M));
	}
	else
	{
		body->SetLinearVelocity(b2Vec2(0.0, speed));
		kinecticBody->setDirection(KinecticBody::Direction::Down);
		kinecticBody->setEndLocation(new b2Vec2(x*P2M, endpoint*P2M));
	}

	KinecticBody::Direction dir = kinecticBody->GetDirection();

	//kinecticBody->addBody(body);


	body->SetUserData(kinecticBody);
//	_kinecticBodies->insert(std::pair<b2Body*, KinecticBody*>(body, kinecticBody));

	return body;
}

void rotateTranslate(b2Vec2& vector, const b2Vec2& center, float angle)
{
	b2Vec2 tmp;
	tmp.x = vector.x*cos(angle) - vector.y*sin(angle);
	tmp.y = vector.x*sin(angle) + vector.y*cos(angle);
	vector = tmp + center;
}

void World::renderBodies(SDL_Renderer *renderer)
{
	//SDL_FillRect(screen, NULL, 0);
	b2Body* tmp = _world->GetBodyList();
	b2Vec2 points[4];

	while (tmp)
	{
		for (int i = 0; i < 4; i++)
		{
			points[i] = ((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->GetVertex(i);
			rotateTranslate(points[i], tmp->GetWorldCenter(), tmp->GetAngle());
		}
		SDL_Rect r;
		r.x = points[0].x * M2P;
		r.y = points[0].y * M2P;
		b2Vec2 *vec = (*_boxSizes)[tmp];
		r.w = vec->x * M2P;
		r.h = vec->y * M2P;

		if (tmp->GetType() == b2_kinematicBody)
		{
			KinecticBody *kinecticBody = static_cast<KinecticBody*>(tmp->GetUserData()); // (*_kinecticBodies)[tmp];

			b2Vec2 *endpoint = kinecticBody->getEndLocation();
			if (kinecticBody->GetDirection() == KinecticBody::Direction::Right)
			{
				if (tmp->GetPosition().x >= kinecticBody->getEndLocation()->x)
				{
					kinecticBody->setDirection(KinecticBody::Direction::Left);
					b2Vec2 vec = b2Vec2(-tmp->GetLinearVelocity().x, 0.0);
					kinecticBody->changeBodiesLinearVelocity(vec);
					tmp->SetLinearVelocity(vec);
				}
			}
			else if (kinecticBody->GetDirection() == KinecticBody::Direction::Left)
			{
				if (tmp->GetPosition().x <= kinecticBody->getStartLocation()->x)
				{
					kinecticBody->setDirection(KinecticBody::Direction::Right);
					b2Vec2 vec = b2Vec2(std::abs(tmp->GetLinearVelocity().x), 0.0);
					kinecticBody->changeBodiesLinearVelocity(vec);
					tmp->SetLinearVelocity(vec);
				}
			}
			else if (kinecticBody->GetDirection() == KinecticBody::Direction::Down)
			{
				if (tmp->GetPosition().y >= kinecticBody->getEndLocation()->y)
				{
					kinecticBody->setDirection(KinecticBody::Direction::Up);
					//tmp->SetLinearVelocity(b2Vec2(0.0, -tmp->GetLinearVelocity().y));


					b2Vec2 vec = b2Vec2(b2Vec2(0.0, -tmp->GetLinearVelocity().y));
					kinecticBody->changeBodiesLinearVelocity(vec);
					tmp->SetLinearVelocity(vec);
				}
			}
			else if (kinecticBody->GetDirection() == KinecticBody::Direction::Up)
			{
				if (tmp->GetPosition().y <= kinecticBody->getStartLocation()->y)
				{
					kinecticBody->setDirection(KinecticBody::Direction::Down);
					//tmp->SetLinearVelocity(b2Vec2(0.0, std::abs(tmp->GetLinearVelocity().y)));



					b2Vec2 vec = b2Vec2(b2Vec2(0.0, std::abs(tmp->GetLinearVelocity().y)));
					kinecticBody->changeBodiesLinearVelocity(vec);
					tmp->SetLinearVelocity(vec);
				}
			}
		}

		// Render rect
		SDL_RenderFillRect(renderer, &r);

		//drawSquare(points, tmp->GetWorldCenter(), tmp->GetAngle());
		tmp = tmp->GetNext();
	}
}


int World::getWidth()
{
	return WIDTH;
}

int World::getHeight()
{
	return HEIGHT;
}
