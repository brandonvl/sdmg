#pragma once
#include "AudioObject.h"

namespace sdmg {
	namespace engine {
		namespace audio {
			class SampleObject :
				public AudioObject
			{
			public:
				SampleObject(AudioEngine &engine);
				virtual ~SampleObject();
				void load(const std::string &fileName, const int &tag) override;
				void play() override;
				void donePlaying(int channel) override;
			private:
				Mix_Chunk *_sample = nullptr;
			};
		}
	}
}