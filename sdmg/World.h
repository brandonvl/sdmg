#pragma once
#include "Box2D\Box2D.h"
#include "sdl\include\SDL.h"
#include "KinecticBody.h"

class World
{
public:
	World(b2Vec2 gravity);
	virtual ~World();

	void renderBodies(SDL_Renderer *renderer);
	void checkKinecticBodies();
	void step(float32 timeStep, int32 velocityIterations, int32 positionIterators);
	void setGravity(b2Vec2 gravity);

	b2Body* addBody(int x, int y, int w, int h, bool dyn);
	b2Body* addKinecticBody(int x, int y, int w, int h, int speed, int endpoint, KinecticBody::Direction direction);

	b2Vec2 getGravity();
	int getWidth();
	int getHeight();
};

