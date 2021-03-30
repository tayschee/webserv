#include "parser.hpp"

void parser::parse_file()
{
	std::ifstream ifs(filename.c_str());
	std::string line;

	while (std::getline(ifs, line))
	{
		std::cout << line << std::endl;
	}
}
