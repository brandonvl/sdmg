#pragma once
#include "AIMachine.h"

namespace sdmg {

	namespace model {
		class Character;
	}

	namespace engine {
		namespace ai {

			class EasyAIMachine : public AIMachine
			{
				public:
					EasyAIMachine(model::Character &controlled, model::Character &enemy);
			};
		}
	}
}