#pragma once
#include <random>
#include <map>
#include <string>
#include <vector> 

namespace sdmg
{
	namespace helperclasses {
		class RandomGenerator
		{
		public:
			int random(const int min, const int max);

			template <typename T>
			const T &randomFromVector(const std::vector<T> &vect) {
				if (vect.size() > 0) {
					int rand = random(0, vect.size() - 1);
					return vect[rand];
				}
				return nullptr;
			}
			RandomGenerator();
			virtual ~RandomGenerator();
		private:
			std::uniform_int_distribution<int> &getOrCreateDist(const int min, const int max);
			std::random_device _dev;
			std::default_random_engine _dre;
			std::map<std::string, std::uniform_int_distribution<int>*> _distMap;
		};
	}
}

