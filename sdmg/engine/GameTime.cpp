//
//
//  @ Project : SDMG
//  @ File Name : GameTime.cpp
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#include "GameTime.h"

namespace sdmg {
	namespace engine {
		void GameTime::update() {
			_lastUpdate = std::chrono::high_resolution_clock::now();
		}
		
		long long GameTime::getElapsedSinceLastUpdate() {
			auto curTime = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::nanoseconds>(curTime - _lastUpdate).count();
		}
		
		long GameTime::getTotalSecondsRunning() {
			auto curTime = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::seconds>(curTime - _start).count();
		}
		
	}
}
