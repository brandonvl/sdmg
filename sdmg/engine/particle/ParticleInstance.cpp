#include "ParticleInstance.h"
#include "ParticleSet.h";

namespace sdmg {
	namespace engine {
		namespace particle {
			ParticleInstance::ParticleInstance(ParticleSet* particleSet, int x, int y) {
				_particleSet = new ParticleSet(*particleSet);
				_x = x;
				_y = y;
			}

			ParticleInstance::~ParticleInstance() {
				delete _particleSet;
			}

			ParticleSet* ParticleInstance::getParticleSet() {
				return _particleSet;
			}

			int ParticleInstance::GetX() {
				return _x;
			}

			int ParticleInstance::GetY() {
				return _y;
			}
		}
	}
}