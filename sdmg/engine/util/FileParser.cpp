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
					bool valueMode = false, stringValue = false, stringFinished = false;

					std::string name, value;

					while (_fin >> noskipws >> ch) {

						if (!valueMode) {
							if (ch != '=') name += ch;
							else valueMode = true;
						}
						else {
							if (ch == '"' && value.size() == 0) stringValue = true;
							else if (ch == '"' && stringValue) stringFinished = true;
							else if (ch == '\n') {
								if (!handle(name, value, valueMode, stringValue, stringFinished)) return false;
							}
							else if ((ch == '"' && !stringValue) || stringFinished) return false;
							else value += ch;
						}
					}

					if (!handle(name, value, valueMode, stringValue, stringFinished)) return false;

					_fin.close();

					return true;
				}
				else return false;
			}

			bool FileParser::handle(std::string &name, std::string &value, bool &valueMode, bool &stringValue, bool &stringFinished) {
				if (stringValue) {
					if (!stringFinished) return false;
					_stringMap.insert(std::make_pair(name, value));
				}
				else {
					float f = std::atof(value.c_str());
					_floatMap.insert(std::make_pair(name, f));
				}

				valueMode = false;
				stringValue = false;
				stringFinished = false;
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
		}
	}
}