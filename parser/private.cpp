#include "parser.hpp"

void parser::parse_file()
{
	std::ifstream ifs(filename.c_str());
	std::string line, name;
	std::vector<std::string> splitted;

	while (std::getline(ifs, line))
	{
		if (line.empty())
			continue;
		splitted = split(line);
		name = splitted[0];
		splitted.erase(splitted.begin());
		main.conf[name] = splitted;
	}

	for (std::map<std::string, std::vector<std::string> >::iterator it = main.conf.begin();
		it != main.conf.end(); it++)
	{
		std::cout << it->first << " : " << it->second[0];
		for (size_t i = 1; i < it->second.size(); i++)
			std::cout << ", " << it->second[i];
		std::cout << std::endl;
	}
}
