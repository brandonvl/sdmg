#pragma once

#include <vector>

namespace sdmg {
	namespace helperclasses {
		class Statistics
		{
		public:
			void save(std::vector<std::vector<std::string>> statistics);
			std::vector<std::vector<std::string>> load();
			void reset();

			static Statistics& getInstance() {
				static Statistics _instance;
				return _instance;
			}

		protected:
			Statistics();

		};
	}
}