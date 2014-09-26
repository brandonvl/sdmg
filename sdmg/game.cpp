#include <iostream>
#include "sdl\include\SDL.h"
#include "Engine.h"
#include <map>
#include <vector>

using namespace SDMG;

class Action {
public:
	virtual void run() = 0;
	virtual Action* create() = 0;
};

class JumpAction : public Action {
public:
	void run()  {
		// do something
		std::cout << "Jumping";
	}
	Action* create() {
		return new JumpAction();
	}
};


std::map<char, Action*> *actions;
std::vector<Action*> *actionContainer;

// adds key binding
void setKeyBinding(char key, Action *actionClass) {
	actions->insert(std::pair<char, Action*>(key, actionClass));
}


// thing to do when an key is pressed
void createAction() {
	// key = 'A'
	Action *action = ((*actions)['A'])->create();
	actionContainer->push_back(action);
}

// run actions
void runActions() {
	// run all actions
	for (int i = 0; i < actionContainer->size(); i++){
		(*actionContainer)[i]->run();
	}
}


int main(int argc, char **argv)
{
	/*
	actions = new std::map<char, Action*>;
	actionContainer = new std::vector<Action*>;

	setKeyBinding('A', new JumpAction());

	createAction();
	runActions();
	*/

	SDL_Keycode code = SDLK_2;

	std::cin.get();
	return 0;
}

