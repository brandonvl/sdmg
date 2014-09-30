#include <iostream>
#include "sdl\include\SDL.h"
#include "engine\physics\PhysicsEngine.h"
#include "model\Character.h"

using namespace sdmg::engine::physics;
using namespace sdmg::model;

int main(int argc, char **argv)
{

	PhysicsEngine *engine = new PhysicsEngine();
	Character *char2 = new Character();

	engine->doAction(char2, PhysicsEngine::Action::MOVELEFT);
	engine->doAction(char2, PhysicsEngine::Action::MOVERIGHT);
	engine->doAction(char2, PhysicsEngine::Action::IDLE);
	engine->doAction(char2, PhysicsEngine::Action::JUMP);


	std::cin.get();
	return 0;
}