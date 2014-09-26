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

namespace sdmg {
	namespace engine {
		class Engine;

		namespace audio {
			class AudioEngine {
			public:
				void play(std::string key);
				void loadSample(std::string key, std::string path);
				void loadMusic(std::string key, std::string path);
				void unload(std::string key);
				void unloadAll();
			private:
				Engine *_engine;
			};
		}
	}
}