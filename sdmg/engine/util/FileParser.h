#pragma once
#include <string>
#include <map>
#include <stack>

namespace sdmg {
	namespace engine {
		namespace util {
			using namespace std;
			class FileParser
			{
			public:
				FileParser();
				virtual ~FileParser();

				void load(std::string path);
				std::string getString(std::string name);
				float getFloat(std::string name);

			private:
				std::map<std::string, std::string> _stringMap;
				std::map<std::string, float> _floatMap;
				void handle(std::string &name, std::string &value, bool &valueMode, bool &stringValue, bool &stringFinished);
			};
		}
	}
}