#include "parser.hpp"

void parser::parse_file()
{
	int file = open(filename.c_str(), O_RDONLY);
	std::string line;
	int line_no = 0;
	blocks::key_type block_id = std::make_pair("server", std::vector<std::string>());

	if (file == -1)
	{
		std::cerr << "Error: " << strerror(errno) << std::endl;
		return;
	}

	_blocks[block_id] = block(block_id.first, block_id.second);
	buffer.assign(BUFFER_SIZE, '\0');
	error = false;

	while (getline(file, line))
	{
		line_no++;

		if (line.empty())
			continue;
		line = remove_comments(line);
		if (line.empty())
			continue;
		line = clean_string(line);
		if (line.empty())
			continue;
		if (!check_line(line, line_no))
		{
			_blocks.clear();
			return;
		}
		parse_line(line, line_no, block_id);
		if (_blocks.empty())
			return;
	}
	close(file);

	buffer.clear();
}

void parser::parse_line(std::string line, int line_no, blocks::key_type &block_id)
{
	std::vector<std::string> splitted;
	bool block = false;
	std::string name;

	if (line[line.length() - 1] == '{')
	{
		block = true;
		line.erase(line.length() - 1);
		line = clean_string(line);
	}
	else if (line == "}")
	{
		block_id = std::make_pair(PARSER_SERVER, std::vector<std::string>());
		return;
	}

	splitted = split(line);
	name = splitted[0];
	splitted.erase(splitted.begin());
	if (block && check_block(name, splitted, line_no))
	{
		if (name == PARSER_LOCATION && splitted[0] != "/")
		{
			if (splitted[0][0] != '/')
				splitted[0].insert(splitted[0].begin(), '/');
			if (splitted[0][splitted[0].length() - 1] == '/')
				splitted[0].erase(splitted[0].length() - 1);
		}
		block_id = std::make_pair(name, splitted);
		_blocks[block_id].create_block(block_id.first, block_id.second, _blocks[entries::value_type(PARSER_SERVER, std::vector<std::string>())]); //dont work
	}
	else if (!block && check_prop(name, block_id.first, splitted, line_no))
	{
		if (name == PARSER_ERROR_PAGE) //this block change
		{
			size_t i(0);

			for (i = 0; i < splitted.size() - 1; i++)
				_blocks[block_id].errors[ft_atoi<int>(splitted[i].c_str())] = splitted[splitted.size() - 1];
		}
		else
			_blocks[block_id].conf[name] = splitted;
	}
	else
		_blocks.clear();
}
