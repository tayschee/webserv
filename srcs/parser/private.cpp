#include "parser.hpp"
#include "message/request.hpp"
#include "message/response.hpp"

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

bool parser::advanced_chk_err_code(const std::vector<std::string> &err, int line_no) const
{
	long nb;
	size_t i(0);
	size_t j;

	while (i < err.size() - 1)
	{
		j = 0;
		while (j < err[i].size())
		{
			if (err[i][j] < '0' || err[i][j] > '9')
			{
				std::cerr << "Error: " << filename << ": an error code takes only numbers. (line: " << line_no << ")\n";
				return false;
			}
			++j;
		}
		nb = ft_atoi<long>(err[i]);
		if (nb < 100 || nb >= 600) //must be change with list of error
		{
			std::cerr << "Error: " << filename << ": An error code must be an integer between 100 and 599. (line: " << line_no << ")\n";
			return false;
		}
		++i;
	}
	if (err[i][0] != '/')
	{
		std::cerr << "Error: " << filename << ": Last element to error_page must be a path strating by '/'. (line: " << line_no << ")\n";
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
	int file2 = open("mime", O_RDONLY); //WARNING
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
			return ;
	}
	close(file);

	while (getline(file2, line))
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

	close(file2);
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
	if (name == PARSER_ACCEPT)
		std::cout << "parsing an accept... block : " << std::boolalpha << block << std::noboolalpha << std::endl;
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
	else{
		_blocks.clear();
	}
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
	std::map<std::string, check_prop_func> prop_checker;

	prop_checker[PARSER_ROOT] = &parser::check_prop_root;
	prop_checker[PARSER_INDEX] = &parser::check_prop_index;
	prop_checker[PARSER_SERVER_NAME] = &parser::check_prop_serv_name;
	prop_checker[PARSER_ACCEPT] = &parser::check_prop_accept;
	prop_checker[PARSER_LISTEN] = &parser::check_prop_listen;
	prop_checker[PARSER_ERROR_PAGE] = &parser::check_prop_err_page;
	prop_checker[PARSER_AUTOINDEX] = &parser::check_prop_autoindex;
	prop_checker[PARSER_SCRIPT_NAME] = &parser::check_prop_script_name;
	prop_checker[PARSER_RETURN] = &parser::check_prop_return;
	prop_checker[PARSER_AUTH_BASIC] = &parser::check_prop_auth_basic;
	prop_checker[PARSER_AUTH_BASIC_USER_FILE] = &parser::check_prop_auth_basic_user_file;
	prop_checker[PARSER_KEEP_ALIVE] = &parser::check_prop_keep_alive;

	try
	{
		if (block_id == "types")
			return (1);
		bool res = (this->*prop_checker.at(name))(block_id, args, line_no);
		error = error | !res;
		return res;
	}
	catch (const std::out_of_range &e)
	{
		std::cout << block_id << std::endl;
		std::cerr << "Error: " << filename << ": Unknown property '" << name << "' at line " << line_no << ".\n";
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

bool parser::check_block_types(const std::vector<std::string>& args, int line_no) const
{
	(void)args;
	(void)line_no;
	return true;
}

bool parser::check_prop_return(const std::string &block_id, const std::vector<std::string> &args, int line_no) const
{
	std::vector<std::string> expected;
	expected.push_back(PARSER_LOCATION);

	if (!basic_chk_block(PARSER_RETURN, block_id, expected, line_no))
		return false;
	if (!basic_chk_args(PARSER_RETURN, args.size(), 2, true, line_no))
		return false;
	return true;;
}

bool parser::check_prop_auth_basic(const std::string &block_id, const std::vector<std::string> &args, int line_no) const
{
	std::vector<std::string> expected;
	expected.push_back(PARSER_LOCATION);

	if (!basic_chk_block(PARSER_AUTH_BASIC, block_id, expected, line_no))
		return false;
	if (!basic_chk_args(PARSER_AUTH_BASIC, args.size(), 1, true, line_no))
		return false;
	return true;
}

bool parser::check_prop_auth_basic_user_file(const std::string &block_id, const std::vector<std::string> &args, int line_no) const
{
	std::vector<std::string> expected;
	expected.push_back(PARSER_LOCATION);

	if (!basic_chk_block(PARSER_AUTH_BASIC_USER_FILE, block_id, expected, line_no))
		return false;
	if (!basic_chk_args(PARSER_AUTH_BASIC_USER_FILE, args.size(), 1, true, line_no))
		return false;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

bool parser::check_block(const std::string &name, const std::vector<std::string>& args, int line_no)
{
	std::map<std::string, check_block_func> block_checker;

	block_checker[PARSER_LOCATION] = &parser::check_block_location;
	block_checker[PARSER_CGI] = &parser::check_block_cgi;
	block_checker[PARSER_TYPES] = &parser::check_block_types;

	try
	{
		bool res = (this->*block_checker.at(name))(args, line_no);
		error = error | !res;
		return res;
	}
	catch (const std::out_of_range &e)
	{
		std::cerr << "Error: " << filename << ": Unknown block '" << name << "' at line " << line_no << ".\n";
		return false;
	}
}

bool parser::check_prop_root(const std::string &block_id, const std::vector<std::string> &args, int line_no) const
{
	std::vector<std::string> expected;
	expected.push_back(PARSER_LOCATION);
	expected.push_back(PARSER_SERVER);
	expected.push_back(PARSER_CGI);

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

bool parser::check_prop_accept(const std::string &block_id, const std::vector<std::string> &args, int line_no) const //pas tester
{
	std::vector<std::string>::const_iterator args_it(args.begin());
	std::vector<std::string>::const_iterator args_end(args.end());

	const parser::blocks::const_iterator server_map(_blocks.find(entries::value_type(PARSER_SERVER, std::vector<std::string>())));
	const entries::const_iterator possible_it(server_map->second.conf.find(PARSER_ACCEPT));
	const entries::const_iterator possible_end(server_map->second.conf.end());
	if (possible_it == possible_end)
	{
		std::cerr << "Error: " << filename << ": Can't check method, server methods are invalid." << " At line " << line_no << std::endl;
			return false;
	}

	const request::method_array possible_method(possible_it->second); //dont work); //request get list of acceptable method back
	request::method_array::const_iterator method_it;
	request::method_array::const_iterator method_end(possible_method.end());

	std::vector<std::string> expected;
	expected.push_back(PARSER_SERVER);
	expected.push_back(PARSER_LOCATION);

	if (!basic_chk_block(PARSER_ACCEPT, block_id, expected, line_no))
		return false;
	if (args_it == args_end) //if there is any argument
	{
		std::cerr << "Error: " << filename << ": 1 method required " << *args_it << " at line " << line_no << std::endl;
		return false;
	}
	while (args_it != args_end)
	{
		method_it = possible_method.begin();
		while (1)
		{
			if (*args_it == *method_it)
				break;
			else if (method_it + 1 == method_end)
			{
				std::cerr << "Error: " << filename << ": Unknow method " << *args_it << " at line " << line_no << std::endl;
				return false;
			}
			++method_it;
		}
		++args_it;
	}
	return true;
}

bool parser::check_prop_listen(const std::string &block_id, const std::vector<std::string> &args, int line_no) const //not test
{
	std::vector<std::string> expected;
	expected.push_back(PARSER_SERVER);

	if (!basic_chk_block(PARSER_LISTEN, block_id, expected, line_no))
		return false;
	if (!basic_chk_args(PARSER_LISTEN, args.size(), 1, true, line_no))
		return false;
	const std::string &number(args[0]);
	char *end;

	int nb = strtol(number.c_str(), &end, 10);

	if (end != (number.c_str() + number.length()))
	{
		std::cerr << "Error: " << filename << ": The port must only contain digits. (line: " << line_no << ")\n";
		return false;
	}
	if (nb < 0 || nb > 65535)
	{
		std::cerr << "Error: " << filename << ": A port is strictly between 0 and 65535. (line: " << line_no << ")\n";
		return false;
	}
	return true;
}

bool parser::check_prop_keep_alive(const std::string &block_id, const std::vector<std::string> &args, int line_no) const //not test
{
	std::vector<std::string> expected;
	expected.push_back(PARSER_SERVER);

	if (!basic_chk_block(PARSER_KEEP_ALIVE, block_id, expected, line_no))
		return false;
	if (!basic_chk_args(PARSER_KEEP_ALIVE, args.size(), 1, true, line_no))
		return false;
	const std::string &number(args[0]);
	char *end;

	int nb = strtol(number.c_str(), &end, 10);

	if (end != (number.c_str() + number.length()))
	{
		std::cerr << "Error: " << filename << ": keep_alive takes only digit in parameter. (line: " << line_no << ")\n";
		return false;
	}
	if (errno != 0)
	{
		std::cerr << "Error: " << filename << ": conversion of parameter failed. (line: " << line_no << ")\n";
		return false;
	}
	if (nb < 0)
	{
		std::cerr << "Error: " << filename << ": Number must be positive. (line: " << line_no << ")\n";
		return false;
	}
	return true;
}

bool parser::check_prop_err_page(const std::string &block_id, const std::vector<std::string> &args, int line_no) const
{
	std::vector<std::string> expected;
	const response::status_array &status_list = response::existing_status;
	const response::status_array::const_iterator end = status_list.end();
	size_t i(0);

	expected.push_back(PARSER_SERVER);
	expected.push_back(PARSER_LOCATION);

	if (!basic_chk_block(PARSER_ERROR_PAGE, block_id, expected, line_no))
		return false;
	if (!basic_chk_args(PARSER_ERROR_PAGE, args.size(), 2, false, line_no)) //change exactly by at least
		return false;
	if (!advanced_chk_err_code(args, line_no)) //tbigot change it
		return false;
	while (i < args.size() - 1)
	{
		if (status_list.find(ft_atoi<size_t>(args[i])) == end)
		{
			std::cerr << "Error: " << filename << ": error status \"" << args[i] << "\" is not implemented. (line: " << line_no << ")\n";
			return false;
		}
		++i;
	}

	return true;
}

bool parser::check_prop_autoindex(const std::string &block_id, const std::vector<std::string> &args, int line_no) const
{
	std::vector<std::string> expected;
	expected.push_back(PARSER_LOCATION);

	if (!basic_chk_block(PARSER_AUTOINDEX, block_id, expected, line_no))
		return false;
	if (!basic_chk_args(PARSER_AUTOINDEX, args.size(), 1, true, line_no))
		return false;
	if (args[0] != "on" && args[0] != "off")
	{
		std::cerr << "Error: " << filename << ": Autoindex takes only two values, that is 'on' or 'off'. (line: " << line_no << ")\n";
		return false;
	}
	return true;
}

bool parser::check_prop_script_name(const std::string &block_id, const std::vector<std::string> &args, int line_no) const
{
	std::vector<std::string> expected;
	expected.push_back(PARSER_CGI);

	if (!basic_chk_block(PARSER_SCRIPT_NAME, block_id, expected, line_no))
		return false;
	if (!basic_chk_args(PARSER_SCRIPT_NAME, args.size(), 1, true, line_no))
		return false;
	return true;
}

bool parser::check_block_location(const std::vector<std::string>& args, int line_no) const
{
	return basic_chk_args(PARSER_LOCATION, args.size(), 1, true, line_no);
}

bool parser::check_block_cgi(const std::vector<std::string>& args, int line_no) const
{
	return basic_chk_args(PARSER_CGI, args.size(), 1, true, line_no);
}

parser::block	parser::create_default_mime_type() const
{
	block	default_mime_type(PARSER_TYPES, std::vector<std::string>());

	default_mime_type.conf.insert(std::make_pair(".html", std::vector<std::string>(1, "text/html")));
	default_mime_type.conf.insert(std::make_pair(".htm", std::vector<std::string>(1, "text/html")));
	default_mime_type.conf.insert(std::make_pair(".bmp", std::vector<std::string>(1, "image/x-ms-bmp")));
	default_mime_type.conf.insert(std::make_pair(".jpg", std::vector<std::string>(1, "image/jpeg")));
	default_mime_type.conf.insert(std::make_pair(".png", std::vector<std::string>(1, "image/png")));

	return default_mime_type;
};
