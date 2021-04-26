#include "parser.hpp"
#include "utils.hpp"

void parser::parse_file()
{
	std::ifstream ifs(filename.c_str());
	std::string line;
	int line_no = 1;
	std::string block_id = "server";

	blocks[block_id] = block(block_id);

	while (std::getline(ifs, line))
	{
		line_no++;
		if (line.empty())
			continue;

		line = clean_string(line);
		if (line.empty())
			continue;

		parse_line(line, line_no, block_id);
	}
}

void parser::parse_line(std::string line, int line_no, std::string &block_id)
{
	std::vector<std::string> splitted;
	bool block = false;
	std::string name;

	(void)line_no;

	if (line[line.length() - 1] == '{')
	{
		block = true;
		line.erase(line.length() - 1);
		line = clean_string(line);
	}
	else if (line == "}")
	{
		block_id = "server";
		return ;
	}

	splitted = split(line);
	name = splitted[0];
	splitted.erase(splitted.begin());
	if (block)
	{
		if (name == "location")
		{
			name = splitted[0];
			splitted.erase(splitted.begin());
		}
		block_id = name;
		blocks[block_id] = parser::block(block_id, splitted);
	}
	else
		blocks[block_id].conf[name] = splitted;
}
