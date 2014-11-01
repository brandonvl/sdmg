#include "Statistics.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

namespace sdmg {
	namespace helperclasses {

		//const string savegame("assets\\statistics\\statistics");

		void Statistics::save(std::vector<std::vector<std::string>> statistics) 
		{
			ofstream output_file("assets\\statistics\\statistics");
			for (auto s : statistics)
			{
				output_file << s.at(0) + " " + s.at(1) + " " + s.at(2) + ";\n";
			}
		}

		std::vector<std::vector<std::string>> Statistics::load()
		{
			std::vector<std::vector<std::string>> statistics;
			std::vector<std::string> character;
			
			std::ifstream input_file("assets\\statistics\\statistics");

			std::string line;
			while (getline(input_file, line)) {
				std::string str;
				for (auto c : line) {
					if (c == ' ' || c == ';') {
						character.push_back(str);
						str = "";
					}
					else {
						str += c;
					}
				}
				statistics.push_back(character);
				character.clear();
			}
			return statistics;
		}
	}
}