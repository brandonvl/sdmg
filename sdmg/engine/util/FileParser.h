#pragma once
#include <string>
#include <map>
#include <stack>
#include <fstream>

namespace sdmg {
	namespace engine {
		namespace util {
			using namespace std;
			class FileParser
			{
			public:
				FileParser();
				virtual ~FileParser();

				bool load(std::string path);
				std::string getString(std::string name);
				float getFloat(std::string name);

			private:
				std::map<std::string, std::string> _stringMap;
				std::map<std::string, float> _floatMap;
				bool handle(std::string &name, std::string &value, bool &valueMode, bool &stringValue, bool &stringFinished);
				fstream _fin;
			};
		}
	}
}