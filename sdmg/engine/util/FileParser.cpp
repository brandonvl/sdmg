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

				try {
					// Check if file is open
					if (_fin.is_open()) {
						resetValues();

						// While, read file per character
						while (_fin >> noskipws >> ch) {

							// If not in value-mode
							if (!_valueMode) {
								// Keep adding the character to the name/key string unit a '=' sign is found
								if (ch != '=') _name += ch;
								else _valueMode = true;
							}
							else handleValueCharacter(ch);
						}

						// Handle the built strings (create key-value pair)
						addToMap();

						// Close stream and return true
						_fin.close();
						return true;
					}
				}
				catch (...) {
					_fin.close();
					return false;
				}
				
				return false;
			}

			void FileParser::handleValueCharacter(char &ch) {
				// Handle value character
				// If the value is empty and a quote if found, the type is string
				if (ch == '"' && _value.size() == 0) _type = ValueType::STRING;
				// If the type is a string and a quote is found, the end of the string is reached
				else if (ch == '"' && _type == ValueType::STRING) _isFinished = true;
				// If the value is empty and a '(' is found, the type is array
				else if (ch == '(' && _value.size() == 0) _type = ValueType::ARRAY;
				// If the type is array and a ')' is found, the array is closed
				else if (ch == ')' && _type == ValueType::ARRAY) _isFinished = true;
				// Key-value pair is closed if a newline is found
				else if (ch == '\n' && (_type == ValueType::NUMBER || _isFinished)) {
					// Handle the built strings (create key-value pair)
					addToMap();
				}
				// Invalid cases:
				// - If a quote is found and the type is not a string
				// - If the current value is closed (and apparently another character is found on the current line)
				else if ((ch == '"' && _type != ValueType::STRING) || _isFinished) {
					throw HandleCharException();
				}
				// Append character to value
				else _value += ch;
			}

			void FileParser::addToMap() {
				switch(_type) {
				case ValueType::STRING:
					if (!_isFinished) throw AddToMapException();
					_stringMap.insert(std::make_pair(_name, _value));
					break;
				case ValueType::ARRAY:
				{
					std::string tmpValue;
					std::vector<float> arr;

					for (int i = 0; i < _value.length(); i++) {
						if (_value[i] == ','){
							arr.push_back(std::atof(tmpValue.c_str()));
							tmpValue.clear();
						}
						else tmpValue += _value[i];
					}

					if (tmpValue.length() > 0) arr.push_back(std::atof(tmpValue.c_str()));

					_arrayMap.insert(std::make_pair(_name, arr));
					break;
				}
				default:
					_floatMap.insert(std::make_pair(_name, std::atof(_value.c_str())));
					break;
				}

				resetValues();
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

			void FileParser::resetValues() {
				_type = ValueType::NUMBER;
				_valueMode = false;
				_isFinished = false;
				_name.clear();
				_value.clear();
			}
		}
	}
}