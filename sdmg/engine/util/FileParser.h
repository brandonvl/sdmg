#pragma once
#include <string>
#include <map>
#include <stack>
#include <fstream>
#include <vector>

namespace sdmg {
	namespace engine {
		namespace util {
			using namespace std;

			class HandleCharException {};
			class AddToMapException {};

			class FileParser
			{
			public:
				FileParser();
				virtual ~FileParser();

				bool load(std::string path);
				std::string getString(std::string name);
				float getFloat(std::string name);
				std::vector<float> getArray(std::string name);

			private:
				enum class ValueType { NUMBER, STRING, ARRAY };
				std::map<std::string, std::string> _stringMap;
				std::map<std::string, float> _floatMap;
				std::map<std::string, std::vector<float>> _arrayMap;
				void addToMap();
				fstream _fin;
				void handleValueCharacter(char &ch);
				void resetValues();

				ValueType _type;
				bool _valueMode, _isFinished;
				std::string _name, _value;
			};
		}
	}
}