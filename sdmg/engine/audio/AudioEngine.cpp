//
//
//  @ Project : SDMG
//  @ File Name : AudioEngine.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//


#include "AudioEngine.h"
#include "AudioObject.h"
#include "MusicObject.h"
#include "SampleObject.h"

sdmg::engine::audio::AudioEngine *aEngine = nullptr;

void donePlayingOut(int channel)
{
	aEngine->donePlaying(channel);
}

namespace sdmg {
	namespace engine {
		namespace audio {

			AudioEngine::AudioEngine() {
				int initFlags = Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);

				if ((initFlags & MIX_INIT_MP3) != MIX_INIT_MP3)
				{
					// do error
				}
				Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

				Mix_AllocateChannels(16);
				Mix_GroupChannels(0, SOUND_EFFECT_CHANNELS-1, SOUND_EFFECT_GROUP_TAG);
				Mix_GroupChannels(SOUND_EFFECT_CHANNELS, VOICE_OVER_CHANNELS-1, VOICE_OVER_GROUP_TAG);

				setVolume(MIX_MAX_VOLUME / 8, AUDIOTYPE::SOUND_EFFECT);
				setVolume(MIX_MAX_VOLUME / 3, AUDIOTYPE::VOICE_OVER);
				setVolume(MIX_MAX_VOLUME / 3, AUDIOTYPE::MUSIC);

				Mix_ChannelFinished(donePlayingOut);

				_audioObjects = std::unordered_map<std::string, AudioObject*>();
				//_audioMutex = SDL_CreateMutex();
				//_audioLoopRunning = true;
				aEngine = this;
			}

			

			AudioEngine::~AudioEngine() {
				unloadAll();

				//SDL_DestroyMutex(_audioMutex);
				//SDL_DetachThread(_audioThread);

				Mix_CloseAudio();
				Mix_Quit();
				aEngine = nullptr;
			}

			void AudioEngine::donePlaying(int channel)
			{
				for (auto it : _audioObjects)
				{
					it.second->donePlaying(channel);
				}
			}

			void AudioEngine::play(const std::string &key, const int &delay) {

				//SDL_LockMutex(_audioMutex);
				if (_audioObjects.find(key) != _audioObjects.end()) {
					//_audioToBePlayed.push(_audioObjects[key]);
					_audioObjects[key]->play();
				}

				//SDL_UnlockMutex(_audioMutex);
			}

			void AudioEngine::load(const std::string &key, const std::string &path, const AUDIOTYPE &audioType) {

				if (_audioObjects.find(key) == _audioObjects.end()) {

					AudioObject *object;
					int groupTag = -1;
					switch (audioType)
					{
						case AUDIOTYPE::MUSIC:
							object = new MusicObject(*this);
							break;
						case AUDIOTYPE::SOUND_EFFECT:
							object = new SampleObject(*this);
							groupTag = 1;
							break;
						case AUDIOTYPE::VOICE_OVER:
							object = new SampleObject(*this);
							groupTag = 2;
							break;
						default:
							object = nullptr;
							break;
					}
					if (object != nullptr) {
						object->load(path, groupTag);
						_audioObjects.insert(std::make_pair(key, object));
					}
				}
			}

			void AudioEngine::audioLoop() {

			}

			void AudioEngine::setVolume(const int &volume, const AUDIOTYPE &audioType) {
				
				switch (audioType)
				{
					case AUDIOTYPE::SOUND_EFFECT:
						_soundEffectsVolume = volume;
						break;
					case AUDIOTYPE::VOICE_OVER:
						_voiceOverVolume = volume;
						break;
					case AUDIOTYPE::MUSIC:
						_musicVolume = volume;
						Mix_VolumeMusic(volume);
						break;
				}
			}

			void AudioEngine::stopMusic() {
				if (Mix_PlayingMusic()) {
					Mix_HaltMusic();
				}
			}

			const int AudioEngine::getFreeChannel(const int &tag)
			{
				return Mix_GroupAvailable(tag);
			}

			void AudioEngine::unload(const std::string &key) {

				if (_audioObjects.find(key) != _audioObjects.end())
					if (_audioObjects[key]->getChannelPlaying() != -1 && _audioObjects[key]->getChannelPlaying() != -2)
						Mix_HaltChannel(_audioObjects[key]->getChannelPlaying());

				delete _audioObjects[key];
				_audioObjects.erase(key);
			}

			void AudioEngine::unloadAll() {

				if (Mix_PlayingMusic())
					Mix_HaltMusic();

				Mix_HaltChannel(-1);

				if (!_audioObjects.empty()) {
					for (auto it = _audioObjects.begin(); it != _audioObjects.end(); it++) {
						delete (*it).second;
					}
					_audioObjects.clear();
				}
			}
			
		}
	}
}
