#include "parser.hpp"

#include <cstdio>
#include <climits>

static void append_buffer(std::string &buffer, std::string &line, size_t limit, bool resize)
{
	line += buffer.substr(0, limit);

	if (resize)
	{
		buffer.erase(0, limit + 1);
		buffer.resize(BUFFER_SIZE, '\0');
	}
	else
		buffer.assign(BUFFER_SIZE, '\0');
}

bool parser::basic_chk_block(const std::string &name, const std::string &actual, const std::vector<std::string> &expected, int line_no) const
{
	std::vector<std::string>::const_iterator it = std::find(expected.begin(), expected.end(), actual);

	if (it == expected.end())
	{
		std::cerr << "Error: " << filename << ": " << name << " should only be in " << expected[0];
		for (size_t i = 1; i < expected.size(); i++)
			std::cerr << (i + 1 < expected.size() ? ", " : " and ") << expected[i];
		std::cerr << ". (line: " << line_no << ")\n";
		return false;
	}
	return true;
}

bool parser::basic_chk_args(const std::string &name, int actual, int expected, bool exact, int line_no) const
{
	std::string message = (exact ? "exactly" : "at least");

	if ((exact && expected != actual) || (!exact && expected > actual))
	{
		std::cerr << "Error: " << filename << ": " << name << " should have " << message << " " << expected << " arguments. (line: " << line_no << ")\n";
		return false;
	}
	return true;
}

bool parser::advanced_chk_err_code(const std::string& err, int line_no) const
{
	char *end;
	long nb = strtol(err.c_str(), &end, 10);

	if (end != (err.c_str() + err.length()))
	{
		std::cerr << "Error: " << filename << ": an error code takes only numbers. (line: " << line_no << ")\n";
		return false;
	}
	if (nb < 100 || nb >= 600)
	{
		std::cerr << "Error: " << filename << ": An error code must be an integer between 100 and 599. (line: " << line_no << ")\n";
		return false;
	}
	return true;
}

bool parser::getline(int fd, std::string &line)
{
	line.clear();
	ssize_t i = 0;
	std::string::size_type limit;

	if ((limit = buffer.find('\n')) != buffer.npos)
	{
		append_buffer(buffer, line, limit, true);
		return true;
	}
	else if (buffer[0] != '\0')
		append_buffer(buffer, line, buffer.find('\0'), false);
	while ((i = read(fd, &buffer[0], BUFFER_SIZE)) > 0 &&
		   (limit = buffer.find('\n')) == buffer.npos)
		append_buffer(buffer, line, limit, false);
	if (i < 0)
		return false;
	append_buffer(buffer, line, limit, true);
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
	if (block)
	{
		if (name == PARSER_LOCATION && splitted[0] != "/")
		{
			if (splitted[0][0] != '/')
				splitted[0].insert(splitted[0].begin(), '/');
			if (splitted[0][splitted[0].length() - 1] == '/')
				splitted[0].erase(splitted[0].length() - 1);
		}
		block_id = std::make_pair(name, splitted);
		_blocks[block_id] = parser::block(block_id.first, block_id.second);
	}
	else if (!block && check_prop(name, block_id.first, splitted, line_no))
	{
		if (name == PARSER_ERROR_PAGE)
			_blocks[block_id].errors[atoi(splitted[0].c_str())] = splitted[1];
		else
			_blocks[block_id].conf[name] = splitted;
	}
	else
		_blocks.clear();
}

std::string parser::find_best_match(std::string arg) const
{
	if (arg.empty() || arg[0] != '/')
		arg.insert(arg.begin(), '/');

	blocks::key_type key = std::make_pair(PARSER_LOCATION, std::vector<std::string>(1, arg));

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

	return pos == line.npos ? line : line.substr(0, pos);
}

bool parser::is_valid() const
{
	if (error)
		return (false);

	blocks::const_iterator it = _blocks.find(std::make_pair(PARSER_LOCATION, std::vector<std::string>(1, "/")));
	if (it == _blocks.end())
	{
		std::cerr << "Error: " << filename << ": No 'location /' block.\n";
		return false;
	}
	else if (it->second.conf.find(PARSER_ROOT) == it->second.conf.end() ||
			 it->second.conf.find(PARSER_INDEX) == it->second.conf.end())
	{
		std::cerr << "Error: " << filename << ": No root or index in the 'location /' block.\n";
		return false;
	}
	return true;
}

bool parser::check_line(const std::string &line, int line_no) const
{
	if (line[0] == '{')
	{
		std::cerr << "Error: " << filename << ": There is a single '{' at line " << line_no << ".\n";
		return false;
	}
	else if (line[line.length() - 1] != '{' && line.find('{') != line.npos)
	{
		std::cerr << "Error: " << filename << " The '{' should be the last character on its line (spaces and comments excluded).\n";
		return false;
	}
	else if (line.find('}') != line.npos && line != "}")
	{
		std::cerr << "Error: " << filename << ": The '}' at line " << line_no << " is not alone.\n";
		return false;
	}
	return true;
}

bool parser::check_prop(const std::string &name, const std::string &block_id, const std::vector<std::string> &args, int line_no)
{
	std::map<std::string, check_func> prop_checker;

	prop_checker[PARSER_ROOT] = &parser::check_prop_root;
	prop_checker[PARSER_INDEX] = &parser::check_prop_index;
	prop_checker[PARSER_SERVER_NAME] = &parser::check_prop_serv_name;
	prop_checker[PARSER_ACCEPT] = &parser::check_prop_accept;
	prop_checker[PARSER_LISTEN] = &parser::check_prop_listen;
	prop_checker[PARSER_ERROR_PAGE] = &parser::check_prop_err_page;
	//prop_checker[PARSER_AUTOINDEX] = &parser::check_prop_autoindex;

	try
	{
		bool res = (this->*prop_checker.at(name))(block_id, args, line_no);
		error = error | !res;
		return res;
	}
	catch (const std::out_of_range &e)
	{
		std::cerr << "Error: " << filename << ": Unknown property '" << name << "' at line " << line_no << ".\n";
		return false;
	}
}

bool parser::check_prop_root(const std::string &block_id, const std::vector<std::string> &args, int line_no) const
{
	std::vector<std::string> expected;
	expected.push_back(PARSER_LOCATION);
	expected.push_back(PARSER_SERVER);

	if (!basic_chk_block(PARSER_ROOT, block_id, expected, line_no))
		return false;
	if (!basic_chk_args(PARSER_ROOT, args.size(), 1, true, line_no))
		return false;
	return true;
}

bool parser::check_prop_index(const std::string &block_id, const std::vector<std::string> &args, int line_no) const
{
	std::vector<std::string> expected;
	expected.push_back(PARSER_LOCATION);

	if (!basic_chk_block(PARSER_INDEX, block_id, expected, line_no))
		return false;
	if (!basic_chk_args(PARSER_INDEX, args.size(), 1, false, line_no))
		return false;
	return true;
}

bool parser::check_prop_serv_name(const std::string &block_id, const std::vector<std::string> &args, int line_no) const
{
	std::vector<std::string> expected;
	expected.push_back(PARSER_SERVER);

	if (!basic_chk_block(PARSER_SERVER_NAME, block_id, expected, line_no))
		return false;
	if (!basic_chk_args(PARSER_SERVER_NAME, args.size(), 1, true, line_no))
		return false;

	std::vector<std::string> splitted = split(args[0], ".");
	if (splitted.size() != 4)
	{
		std::cerr << "Error: " << filename << ": The IPv4 adress must be in dotted-decimal notation. (line: " << line_no << ")\n";
		return false;
	}
	for (size_t i = 0; i < splitted.size(); i++)
	{
		char *end;
		int val = strtol(splitted[i].c_str(), &end, 10);

		if (end != (splitted[i].c_str() + splitted[i].length()))
		{
			std::cerr << "Error: " << filename << ": An IP adress should contain no letter. (line = " << line_no << ")\n";
			return false;
		}
		if (errno == ERANGE || !(0 <= val && val <= 255))
		{
			std::cerr << "Error: " << filename << ": out of range number. (line: " << line_no << ")\n";
			return false;
		}
	}
	return true;
}

bool parser::check_prop_accept(const std::string &block_id, const std::vector<std::string> &args, int line_no) const
{
	(void)args;
	(void)line_no;
	(void)block_id;
	return true;
}

bool parser::check_prop_listen(const std::string &block_id, const std::vector<std::string> &args, int line_no) const
{
	(void)args;
	(void)line_no;
	(void)block_id;
	return true;
}

bool parser::check_prop_err_page(const std::string &block_id, const std::vector<std::string> &args, int line_no) const
{
	std::vector<std::string> expected;
	expected.push_back(PARSER_SERVER);
	expected.push_back(PARSER_LOCATION);

	if (!basic_chk_block(PARSER_ERROR_PAGE, block_id, expected, line_no))
		return false;
	if (!basic_chk_args(PARSER_ERROR_PAGE, args.size(), 2, true, line_no))
		return false;
	if (!advanced_chk_err_code(args[0], line_no))
		return false;
	return true;
}
