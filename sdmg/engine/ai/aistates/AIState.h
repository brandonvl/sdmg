#pragma once

#include <string>

namespace sdmg {

	namespace model {
		class Character;
	}

	namespace engine {
		class GameTime;
		class GameBase;
		namespace ai {
			class AIMachine;
			namespace aistate {
				class AIState
				{
					public:
						AIState(const std::string &keyName, AIMachine &machine) : _key(keyName), _machine(&machine) { };
						virtual ~AIState() { };
						virtual bool isComplete() { return _transition != ""; };
						std::string getStateKey() { return _key; };
						std::string getTransition() { return _transition; };
						virtual void enter(model::Character &controlled, GameTime &gameTime, GameBase &game) = 0;
						virtual void update(model::Character &controlled, GameTime &gameTime, GameBase &game) = 0;
						virtual void exit(model::Character &controlled, GameTime &gameTime, GameBase &game) = 0;
					protected:
						AIMachine *_machine = nullptr;
						std::string _transition = "";
						std::string _key;
				};
			}
		}
	}
};