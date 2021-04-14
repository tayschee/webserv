#include "parser.hpp"

void parser::parse_file()
{
	std::ifstream ifs(filename.c_str());
	std::string line, name;
	std::vector<std::string> splitted;
	int number = 1;
	bool block;

	while (std::getline(ifs, line))
	{
		block = false;
		if (line.empty())
		{
			number++;
			continue;
		}
		else if (isspace(line[0]))
		{
			std::cerr << "Error: Unexpected whitespace at " << filename << ":" << number << std::endl;
			return;
		}
		else if (line[line.length() - 1] == '{')
		{
			line.erase(line.length() - 1);
			block = true;
		}
		splitted = split(line);
		name = splitted[0];
		splitted.erase(splitted.begin());
		number++;
		if (!block)
			main.conf[name] = splitted;
		else
		{
			struct block *b = new struct block(name, splitted, &main);
			parse_block(ifs, b, number);
			main.add_block(b);
		}
	}
}

void parser::parse_block(std::ifstream &ifs, parser::block *b, int &number)
{
	std::string line, name;
	std::vector<std::string> splitted;
	bool block;

	while (std::getline(ifs, line))
	{
		block = false;
		if (line.find("}") != line.npos)
			break;
		if (line.empty())
		{
			continue;
		}
		else if (line[line.length() - 1] == '{')
		{
			line.erase(line.length() - 1);
			block = true;
		}
		splitted = split(line);
		name = splitted[0];
		splitted.erase(splitted.begin());
		number++;
		if (!block)
			b->conf[name] = splitted;
		else
		{
			struct block *subblock = new struct block(name, splitted, b);
			parse_block(ifs, subblock, number);
			b->add_block(subblock);
		}
	}
	if (line == "}")
		number++;
}
