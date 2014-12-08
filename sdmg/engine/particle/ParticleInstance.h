#pragma once

namespace sdmg {
	namespace engine {
		namespace particle {
			class ParticleSet;
			class ParticleInstance
			{
			public:
				ParticleInstance(ParticleSet* particleSet, int x, int y);
				virtual ~ParticleInstance();
				ParticleSet* getParticleSet();
				int GetX();
				int GetY();
			private:
				int _x, _y;
				ParticleSet* _particleSet;
			};
		}
	}
}

