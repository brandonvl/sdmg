#include "RandomGenerator.h"
#include <random>

namespace sdmg
{
	namespace helperclasses {


		RandomGenerator::RandomGenerator() : _dre(_dev())
		{
		}

		RandomGenerator::~RandomGenerator()
		{
			for (auto p : _distMap) {
				delete p.second;
				p.second = nullptr;
			}
		}

		int RandomGenerator::random(int min, int max) {
			return getOrCreateDist(min, max)(_dre);
		}

		std::uniform_int_distribution<int> &RandomGenerator::getOrCreateDist(const int min, const int max) {
			std::string key = std::to_string(min) + "_" + std::to_string(max);
			if (!_distMap.count(key))
				_distMap.insert(std::make_pair(key, new std::uniform_int_distribution<int>(min, max)));
			return *_distMap[key];
		}
	}
}
