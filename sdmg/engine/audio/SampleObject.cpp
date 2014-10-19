#include "SampleObject.h"
#include "AudioEngine.h"

namespace sdmg {
	namespace engine {
		namespace audio {
			SampleObject::SampleObject(AudioEngine &engine) : AudioObject(engine)
			{
				_sample = nullptr;
			}


			SampleObject::~SampleObject()
			{
				Mix_FreeChunk(_sample);
				_sample = nullptr;
			}

			void SampleObject::load(const std::string &fileName, const int &tag)
			{
				_sample = Mix_LoadWAV(fileName.c_str());
				_groupTag = tag;
			}

			void SampleObject::play()
			{
				_channelPlaying = _audioEngine->getFreeChannel(_groupTag);

				if (_channelPlaying != -1)
					Mix_PlayChannel(_channelPlaying, _sample, 1);
			}

			void SampleObject::donePlaying(int channel)
			{
				if (channel == _channelPlaying)
					_channelPlaying = -1;
			}
		}
	}
}
