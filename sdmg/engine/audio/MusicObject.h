#pragma once
#include "AudioObject.h"

namespace sdmg {
	namespace engine {
		namespace audio {
			class MusicObject :
				public AudioObject
			{
			public:
				MusicObject(AudioEngine &engine);
				virtual ~MusicObject();
				void load(const std::string &fileName, const int &tag) override;
				void play() override;
			private:
				Mix_Music *_music = nullptr;
			};
		}
	}
}