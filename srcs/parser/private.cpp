#include "parser.hpp"

#include <cstdio>

bool parser::getline(int fd, std::string &line)
{
	line.clear();
	ssize_t i = 0;
	std::string::size_type limit;

	if ((limit = buffer.find('\n')) != buffer.npos)
	{
		line += buffer.substr(0, limit);
		buffer.erase(0, limit + 1);
		buffer.resize(BUFFER_SIZE, '\0');
		return true;
	}
	else if (buffer[0] != '\0')
	{
		line += buffer.substr(0, buffer.find('\0'));
		buffer.assign(BUFFER_SIZE, '0');
	}
	while ((i = read(fd, &buffer[0], BUFFER_SIZE)) > 0 &&
			(limit = buffer.find('\n')) == buffer.npos)
	{
		line += buffer;
		buffer.assign(BUFFER_SIZE, '\0');
	}
	if (i < 0)
		return false;
	line += buffer.substr(0, limit);
	buffer.erase(0, limit + 1);
	buffer.resize(BUFFER_SIZE, '\0');
	if ((limit = line.find('\0')) != line.npos)
		line.erase(limit);
	return i > 0;
}
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
	}

	close(file);
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

std::string parser::remove_comments(const std::string &line) const
{
	std::string::size_type pos = line.find('#');

	return pos == line.npos ? line : line.substr(pos);
}

bool parser::is_valid() const
{
	blocks::const_iterator it = _blocks.find(std::make_pair("location", std::vector<std::string>(1, "/")));
	if (it == _blocks.end())
	{
		std::cerr << "Error: " << filename << ": No 'location /' block" << std::endl;
		return false;
	}
	else if (it->second.conf.find("root") == it->second.conf.end() &&
			 it->second.conf.find("index") == it->second.conf.end())
	{
		std::cerr << "Error: " << filename << ": No root or index in the 'location /' block" << std::endl;
		return false;
	}
	return true;
}

bool parser::check_line(const std::string &line, int line_no) const
{
	if (line[0] == '{')
	{
		std::cerr << "Error: " << filename << ": There is a single '{' at line " << line_no << std::endl;
		return false;
	}
	else if (line[line.length() - 1] != '{' && line.find('{') != line.npos)
	{
		std::cerr << "Error: " << filename << " The '{' should be the last character on its line (spaces and comments excluded)." << std::endl;
		return false;
	}
	else if (line.find('}') != line.npos && line != "}")
	{
		std::cerr << "Error: " << filename << ": The '}' at line " << line_no << " is not alone." << std::endl;
		return false;
	}
	return true;
}

bool parser::check_prop(const std::string &name, const std::vector<std::string> &args, int line_no) const
{
	std::map<std::string, check_func> prop_checker;

	prop_checker["root"] = &parser::check_prop_root;
	prop_checker["index"] = &parser::check_prop_index;
	prop_checker["server_name"] = &parser::check_prop_serv_name;
	prop_checker["return"] = &parser::check_prop_return;
	prop_checker["accept"] = &parser::check_prop_accept;
	prop_checker["listen"] = &parser::check_prop_listen;
	prop_checker["error_page"] = &parser::check_prop_err_page;

	try
	{
		return (this->*prop_checker.at(name))(args, line_no);
	}
	catch (const std::out_of_range &e)
	{
		std::cerr << "Error: " << filename << ": Unknown property '" << name << "' at line " << line_no << "." << std::endl;
		return false;
	}
}

bool parser::check_prop_root(const std::vector<std::string> &args, int line_no) const
{
	(void)args;
	(void)line_no;
	return true;
}

bool parser::check_prop_index(const std::vector<std::string> &args, int line_no) const
{
	(void)args;
	(void)line_no;
	return true;
}

bool parser::check_prop_serv_name(const std::vector<std::string> &args, int line_no) const
{
	(void)args;
	(void)line_no;
	return true;
}

bool parser::check_prop_return(const std::vector<std::string> &args, int line_no) const
{
	(void)args;
	(void)line_no;
	return true;
}

bool parser::check_prop_accept(const std::vector<std::string> &args, int line_no) const
{
	(void)args;
	(void)line_no;
	return true;
}

bool parser::check_prop_listen(const std::vector<std::string> &args, int line_no) const
{
	(void)args;
	(void)line_no;
	return true;
}

bool parser::check_prop_err_page(const std::vector<std::string> &args, int line_no) const
{
	(void)args;
	(void)line_no;
	return true;
}
