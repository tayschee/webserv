#include "parser.hpp"
#include "message/request.hpp"
#include "message/response.hpp"

bool parser::check_prop(const std::string &name, const std::string &block_id, const std::vector<std::string> &args, int line_no)
{
	std::map<std::string, check_prop_func> prop_checker;

	prop_checker[PARSER_ROOT] = &parser::check_prop_root;
	prop_checker[PARSER_INDEX] = &parser::check_prop_index;
	prop_checker[PARSER_HOST] = &parser::check_prop_host;
	prop_checker[PARSER_ACCEPT] = &parser::check_prop_accept;
	prop_checker[PARSER_LISTEN] = &parser::check_prop_listen;
	prop_checker[PARSER_ERROR_PAGE] = &parser::check_prop_err_page;
	prop_checker[PARSER_AUTOINDEX] = &parser::check_prop_autoindex;
	prop_checker[PARSER_SCRIPT_NAME] = &parser::check_prop_script_name;
	prop_checker[PARSER_RETURN] = &parser::check_prop_return;
	prop_checker[PARSER_AUTH_BASIC] = &parser::check_prop_auth_basic;
	prop_checker[PARSER_AUTH_BASIC_USER_FILE] = &parser::check_prop_auth_basic_user_file;
	prop_checker[PARSER_KEEP_ALIVE] = &parser::check_prop_keep_alive;
	prop_checker[PARSER_SERVER_NAME] = &parser::check_prop_serv_name;
	prop_checker[PARSER_BODY_SIZE_MAX] = &parser::check_prop_body_size_max;
	prop_checker[PARSER_ALIAS] = &parser::check_prop_alias;

	try
	{
		if (block_id == PARSER_TYPES)
			return (1);
		bool res = (this->*prop_checker.at(name))(block_id, args, line_no);
		error = error | !res;
		return res;
	}
	catch (const std::out_of_range &e)
	{
		std::cerr << e.what() << "\n\n";
		std::cerr << "Error: " << filename << ": Unknown property '" << name << "' at line " << line_no << ".\n";
		error = true;
		return false;
	}
}

bool parser::check_prop_return(const std::string &block_id, const std::vector<std::string> &args, int line_no) const
{
	std::vector<std::string> expected;
	expected.push_back(PARSER_LOCATION);

	if (!basic_chk_block(PARSER_RETURN, block_id, expected, line_no))
		return false;
	if (!basic_chk_args(PARSER_RETURN, args.size(), 2, true, line_no))
		return false;
	if (!advanced_chk_err_code(args, line_no))
		return false;
	return true;
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
	expected.push_back(PARSER_SERVER);

	if (!basic_chk_block(PARSER_INDEX, block_id, expected, line_no))
		return false;
	if (!basic_chk_args(PARSER_INDEX, args.size(), 1, false, line_no))
		return false;
	return true;
}

bool parser::check_prop_host(const std::string &block_id, const std::vector<std::string> &args, int line_no) const
{
	std::vector<std::string> expected;
	expected.push_back(PARSER_SERVER);

	if (!basic_chk_block(PARSER_HOST, block_id, expected, line_no))
		return false;
	if (!basic_chk_args(PARSER_HOST, args.size(), 1, true, line_no))
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
		int val;

		try
		{
			val = ft_strtol(splitted[i].c_str(), &end, 10);
		}
		catch (const std::out_of_range &e)
		{
			std::cerr << "Error: " << filename << ": out of range number. (line: " << line_no << ")\n";
			return false;
		}

		if (end != (splitted[i].c_str() + splitted[i].length()))
		{
			std::cerr << "Error: " << filename << ": An IP address should contain no letter. (line = " << line_no << ")\n";
			return false;
		}
		if (val > 255 || val < 0)
		{
			std::cerr << "Error: " << filename << ": An IP address should only contain numbers between 0 and 255. (line: " << line_no << ")\n";
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
		std::cerr << "Error: " << filename << ": Can't check method, server methods are invalid."
				  << " At line " << line_no << std::endl;
		return false;
	}

	const request::method_array possible_method(possible_it->second); //dont work); //request get list of acceptable method back
	request::method_array::const_iterator method_it;
	request::method_array::const_iterator method_end(possible_method.end());

	std::vector<std::string> expected;
	expected.push_back(PARSER_SERVER);
	expected.push_back(PARSER_LOCATION);
	expected.push_back(PARSER_CGI);

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
				std::cerr << "Error: " << filename << ": Unknown method " << *args_it << " at line " << line_no << std::endl;
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
	if (!basic_chk_args(PARSER_LISTEN, args.size(), 1, false, line_no))
		return false;
	if (args.size() > 2)
	{
		std::cerr << "Error: " << filename << ": Listen takes between 1 and 2 arguments. (line: " << line_no << ")\n";
		return false;
	}
	const std::string &number(args[0]);
	char *end;
	int nb;

	try
	{
		nb = ft_strtol(number.c_str(), &end, 10);
	}
	catch (const std::out_of_range &e)
	{
		std::cerr << "Error: " << filename << ": out of range number. (line: " << line_no << ")\n";
		return false;
	}

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
	if (args.size() == 2 && args[1] != "default_server")
	{
		std::cerr << "Error: " << filename << ": The second argument of listen can only be default_server. (line: " << line_no << ")\n";
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
	long nb;

	try
	{
		nb = ft_strtol(number.c_str(), &end, 10);
	}
	catch (const std::out_of_range &e)
	{
		std::cerr << "Error: " << filename << ": out of range number. (line: " << line_no << ")\n";
		return false;
	}

	if (end != (number.c_str() + number.length()))
	{
		std::cerr << "Error: " << filename << ": keep_alive takes only digit in parameter. (line: " << line_no << ")\n";
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
	expected.push_back(PARSER_SERVER);

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

bool parser::check_prop_serv_name(const std::string &block_id, const std::vector<std::string> &args, int line_no) const
{
	std::vector<std::string> expected;
	expected.push_back(PARSER_SERVER);

	if (!basic_chk_block(PARSER_SERVER_NAME, block_id, expected, line_no))
		return false;
	if (!basic_chk_args(PARSER_SERVER_NAME, args.size(), 1, true, line_no))
		return false;

	std::map<std::string, ServerNameEntry>::const_iterator it = names.find(args[0]);
	blocks::key_type key = std::make_pair(PARSER_SERVER, std::vector<std::string>());
	std::string host = _blocks.at(key).conf.at(PARSER_HOST)[0];
	std::string port = _blocks.at(key).conf.at(PARSER_LISTEN)[0];
	if (it != names.end() && port == it->second.port && host == it->second.host)
	{
		ServerNameEntry entry = it->second;

		std::cerr << "Error: " << filename << ": The name '" << args[0] << "' is already used in " << entry.filename << " with the same host and port:" << entry.line_no << ". (line: " << line_no << ")\n";
		return false;
	}
	return true;
}

bool parser::check_prop_body_size_max(const std::string &block_id, const std::vector<std::string> &args, int line_no) const
{
	// std::vector<std::string> expected;
	// expected.push_back(PARSER_SERVER);

	// if (!basic_chk_block(PARSER_BODY_SIZE_MAX, block_id, expected, line_no))
	// 	return false;
	// if (!basic_chk_args(PARSER_BODY_SIZE_MAX, args.size(), 1, true, line_no))
	// 	return false;

	// std::string number = args[0];
	// char *end;
	// int nb;

	// try
	// {
	// 	nb = ft_strtol(number.c_str(), &end, 10);
	// }
	// catch (const std::out_of_range &e)
	// {
	// 	std::cerr << "Error: " << filename << ": out of range number. (line: " << line_no << ")\n";
	// 	return false;
	// }

	// if (end != (number.c_str() + number.length()))
	// {
	// 	std::cerr << "Error: " << filename << ": keep_alive takes only digit in parameter. (line: " << line_no << ")\n";
	// 	return false;
	// }
	// if (nb < 0)
	// {
	// 	std::cerr << "Error: " << filename << ": Number must be positive. (line: " << line_no << ")\n";
	// 	return false;
	// }
	(void)block_id;
	(void)args;
	(void)line_no;
	return true;
}

bool parser::check_prop_alias(const std::string &block_id, const std::vector<std::string> &args, int line_no) const
{
	std::vector<std::string> expected;
	expected.push_back(PARSER_LOCATION);

	if (!basic_chk_block(PARSER_ALIAS, block_id, expected, line_no))
		return false;
	if (!basic_chk_args(PARSER_ALIAS, args.size(), 1, true, line_no))
		return false;
	return true;
}

bool parser::check_block(const std::string &name, const std::vector<std::string> &args, int line_no)
{
	std::map<std::string, check_block_func> block_checker;

	block_checker[PARSER_LOCATION] = &parser::check_block_location;
	block_checker[PARSER_CGI] = &parser::check_block_cgi;

	try
	{
		bool res = (this->*block_checker.at(name))(args, line_no);
		error = error | !res;
		return res;
	}
	catch (const std::out_of_range &e)
	{
		std::cerr << "Error: " << filename << ": Unknown block '" << name << "' at line " << line_no << ".\n";
		error = true;
		return false;
	}
}

bool parser::check_block_location(const std::vector<std::string> &args, int line_no) const
{
	return basic_chk_args(PARSER_LOCATION, args.size(), 1, true, line_no);
}

bool parser::check_block_cgi(const std::vector<std::string> &args, int line_no) const
{
	return basic_chk_args(PARSER_CGI, args.size(), 1, true, line_no);
}
