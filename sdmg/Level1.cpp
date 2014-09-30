#include "Level1.h"
#include <World.h>
#include "KinecticBody.h"

Level1::Level1()
{
}


Level1::~Level1()
{
}

void Level1::loadLevel(World *world)
{
	world->setGravity(b2Vec2(0.0, 100.0));

	world->addBody(world->getWidth() / 2, world->getHeight() - 50, world->getWidth(), 30, false);


	world->addBody(world->getWidth() / 4, world->getHeight() - 200, world->getWidth() / 2, 30, false);





	world->addKinecticBody(800, 200, 200, 30, 20, 600, KinecticBody::Direction::Down);

	world->addKinecticBody(200, 200, 200, 30, 20, 600, KinecticBody::Direction::Right);

}
