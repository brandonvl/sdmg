#include "FileParser.h"
#include <fstream>
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

			void FileParser::load(std::string path) {
				char ch;
				fstream fin(path, fstream::in);

				bool valueMode = false, stringValue = false, stringFinished = false;

				std::string name, value;

				while (fin >> noskipws >> ch) {

					if (!valueMode) {
						if (ch != '=') name += ch;
						else valueMode = true;
					}
					else {
						if (ch == '"' && value.size() == 0) stringValue = true;
						else if (ch == '"' && stringValue) stringFinished = true;
						else if (ch == '\n') {
							handle(name, value, valueMode, stringValue, stringFinished);
						}
						else if ((ch == '"' && !stringValue) || stringFinished) throw exception("Unexpected " + ch);
						else value += ch;
					}
				}

				handle(name, value, valueMode, stringValue, stringFinished);
			}

			void FileParser::handle(std::string &name, std::string &value, bool &valueMode, bool &stringValue, bool &stringFinished) {
				if (stringValue) {
					if (!stringFinished) throw exception("String not closed");
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