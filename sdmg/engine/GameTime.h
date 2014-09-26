//
//
//  @ Project : SDMG
//  @ File Name : GameTime.h
//  @ Date : 26-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include <chrono>

namespace sdmg {
	namespace engine {
		class GameTime {
		public:
			void update();
			long long getElapsedSinceLastUpdate();
			long getTotalSecondsRunning();
		private:
			std::chrono::high_resolution_clock::time_point _start;
			std::chrono::high_resolution_clock::time_point _lastUpdate;
		};
	}
}