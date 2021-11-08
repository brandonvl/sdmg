//
//
//  @ Project : SDMG
//  @ File Name : AudioEngine.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include <string>
#include <queue>
#include <unordered_map>



namespace sdmg {
	namespace engine {
		class Engine;

		namespace audio {
			class AudioObject;
			enum class AUDIOTYPE
			{
				SOUND_EFFECT = 1,
				VOICE_OVER = 2,
				MUSIC = 4
			};
			class AudioEngine {
			public:
				AudioEngine();
				virtual ~AudioEngine();
				void play(const std::string &key, const int &delay);
				void load(const std::string &key, const std::string &path, const AUDIOTYPE &audioType);
				void setVolume(const int &volume, const AUDIOTYPE &audioType);
				const int getMusicVolume() { return _musicVolume; }
				const int getSoundEffectVolume() { return _soundEffectsVolume; }
				const int getVoiceOverVolume() { return _voiceOverVolume; }
				void loadMusic(const std::string &key, const std::string &path);
				void stopMusic();
				void unload(const std::string &key);
				void unloadAll();
				const int getFreeChannel(const int &tag);
				void donePlaying(int channel);
			private:
				const int CHANNELS_TO_INIT = 16;
				const int SOUND_EFFECT_CHANNELS = 8;
				const int VOICE_OVER_CHANNELS = 8;

				const int SOUND_EFFECT_GROUP_TAG = 1;
				const int VOICE_OVER_GROUP_TAG = 2;

				int _musicVolume;
				int _soundEffectsVolume;
				int _voiceOverVolume;

				std::queue<AudioObject*> _audioToBePlayed;
				//SDL_Thread *_audioThread;
				//SDL_mutex *_audioMutex;
				bool _audioLoopRunning;

				Engine *_engine = nullptr;
				std::unordered_map<std::string,AudioObject*> _audioObjects;
				void audioLoop();
			};
		}
	}
}