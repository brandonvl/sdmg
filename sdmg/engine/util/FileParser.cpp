#include "FileParser.h"
#include <iostream>
#include <sstream>

namespace sdmg {
	namespace engine {
		namespace util {
			FileParser::FileParser()
			{
			}


			FileParser::~FileParser()
			{
			}

			bool FileParser::load(std::string path) {
				char ch;
				_fin = fstream(path, fstream::in);

				if (_fin.is_open()) {

					ValueType type = ValueType::NUMBER;
					bool valueMode = false, isFinished = false;

					std::string name, value;

					while (_fin >> noskipws >> ch) {

						if (!valueMode) {
							if (ch != '=') name += ch;
							else valueMode = true;
						}
						else {
							if (ch == '"' && value.size() == 0) type = ValueType::STRING;
							else if (ch == '"' && type == ValueType::STRING) isFinished = true;
							else if (ch == '(' && value.size() == 0) type = ValueType::ARRAY;
							else if (ch == ')' && type == ValueType::ARRAY) isFinished = true;
							else if (ch == '\n') {
								if (!handle(name, value, valueMode, type, isFinished)) return false;
							}
							else if ((ch == '"' && type != ValueType::STRING) || isFinished) return false;
							else value += ch;
						}
					}

					if (!handle(name, value, valueMode, type, isFinished)) return false;

					_fin.close();

					return true;
				}
				else return false;
			}

			bool FileParser::handle(std::string &name, std::string &value, bool &valueMode, ValueType &valueType, bool &isFinished) {
				
				
				switch(valueType) {
				case ValueType::STRING:
					if (!isFinished) return false;
					_stringMap.insert(std::make_pair(name, value));
					break;
				case ValueType::ARRAY:
				{
					std::string tmpValue;
					std::vector<float> arr;

					for (int i = 0; i < value.length(); i++) {
						if (value[i] == ','){
							arr.push_back(std::atof(tmpValue.c_str()));
							tmpValue.clear();
						}
						else tmpValue += value[i];
					}

					if (tmpValue.length() > 0) arr.push_back(std::atof(tmpValue.c_str()));

					_arrayMap.insert(std::make_pair(name, arr));
					break;
				}
				default:
					_floatMap.insert(std::make_pair(name, std::atof(value.c_str())));
					break;
				}
				
				valueMode = false;
				valueType = ValueType::NUMBER;
				isFinished = false;
				name.clear();
				value.clear();
			}

			std::string FileParser::getString(std::string name) {
				if (_stringMap.count(name))
					return _stringMap[name];
				return "";
			}

			float FileParser::getFloat(std::string name) {
				if (_floatMap.count(name))
					return _floatMap[name];
				return 0;
			}

			std::vector<float> FileParser::getArray(std::string name) {
				if (_arrayMap.count(name))
					return _arrayMap[name];
				return vector<float>(0);
			}
		}
	}
}