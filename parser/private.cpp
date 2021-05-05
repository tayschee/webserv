#include "parser.hpp"

void parser::parse_file()
{
	std::ifstream ifs(filename.c_str());
	std::string line;
	int line_no = 0;
	blocks::key_type block_id = std::make_pair("server", std::vector<std::string>());

	_blocks[block_id] = block(block_id.first, block_id.second);

	while (std::getline(ifs, line))
	{
		line_no++;
		if (line.empty())
			continue;

		line = clean_string(line);
		if (line.empty())
			continue;
		if (line[0] == '{')
		{
			_blocks.clear();
			std::cerr << "Error: " << filename << ": There is a single '{' at line " << line_no << std::endl;
			return ;
		}
		else if (line.find('}') != line.npos && line != "}")
		{
			_blocks.clear();
			std::cerr << "Error: " << filename << ": The '}' at line " << line_no << " is not alone." << std::endl;
			return ;
		}

		parse_line(line, line_no, block_id);
	}
}

void parser::parse_line(std::string line, int line_no, blocks::key_type &block_id)
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
		block_id = std::make_pair("server", std::vector<std::string>());
		return;
	}

	splitted = split(line);
	name = splitted[0];
	splitted.erase(splitted.begin());
	if (block)
	{
		if (name == "location" && splitted[0] != "/")
		{
			if (splitted[0][0] != '/')
				splitted[0].insert(splitted[0].begin(), '/');
			if (splitted[0][splitted[0].length() - 1] == '/')
				splitted[0].erase(splitted[0].length() - 1);
		}
		block_id = std::make_pair(name, splitted);
		_blocks[block_id] = parser::block(block_id.first, block_id.second);
	}
	else
		_blocks[block_id].conf[name] = splitted;
}

std::string parser::find_best_match(std::string arg) const
{
	if (arg.empty() || arg[0] != '/')
		arg.insert(arg.begin(), '/');

	blocks::key_type key = std::make_pair("location", std::vector<std::string>(1, arg));

	if (_blocks.find(key) != _blocks.end())
		return arg;

	while (!arg.empty())
	{
		key.second[0] = arg;
		if (_blocks.find(key) != _blocks.end() || arg == "/")
			return arg;
		arg.erase(arg.rfind('/'));
	}
	return "/";
}

bool parser::is_valid() const
{
	if (_blocks.find(std::make_pair("location", std::vector<std::string>(1, "/"))) == _blocks.end())
	{
		std::cerr << "Error: " << filename << ": No 'location /' block" << std::endl;
		return false;
	}
	return true;
}
