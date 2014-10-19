#pragma once
#include <string>
#include "sdl\include\SDL_mixer.h"

namespace sdmg {
	namespace engine {
		namespace audio {
			class AudioEngine;
			class AudioObject
			{
			public:
				virtual ~AudioObject() {};
				virtual void load(const std::string &fileName, const int &tag) = 0;
				virtual void play() = 0;
				virtual const int &getChannelPlaying() { return _channelPlaying; }
				virtual void donePlaying(int channel) { }
			protected:
				int _channelPlaying;
				int _groupTag;
				AudioEngine *_audioEngine;
				AudioObject(AudioEngine &engine) : _audioEngine(&engine) { }
				
			};
		}
	}
}

