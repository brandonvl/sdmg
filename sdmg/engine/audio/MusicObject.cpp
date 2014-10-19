#include "MusicObject.h"
#include "AudioEngine.h"


namespace sdmg {
	namespace engine {
		namespace audio {
			MusicObject::MusicObject(AudioEngine &engine) : AudioObject(engine)
			{
				_music = nullptr;
				_channelPlaying = -2;
			}

			MusicObject::~MusicObject()
			{
				Mix_FreeMusic(_music);
				_music = nullptr;
			}

			void MusicObject::load(const std::string &fileName, const int &tag)
			{
				_music = Mix_LoadMUS(fileName.c_str());
			}

			void MusicObject::play()
			{
				Mix_PlayMusic(_music, -1);
			}
		}
	}
}