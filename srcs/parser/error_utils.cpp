#include "parser.hpp"

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
		if (nb < 300 || nb >= 600) //must be change with list of error
		{
			std::cerr << "Error: " << filename << ": An error code must be an integer between 300 and 599. (line: " << line_no << ")\n";
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

bool parser::validate()
{
	if (error || _blocks.empty())
		return (false);

	blocks::key_type server = std::make_pair(PARSER_SERVER, std::vector<std::string>());
	_blocks[server].conf.insert(std::make_pair(PARSER_LISTEN, std::vector<std::string>(1, "80")));

	blocks::const_iterator it = _blocks.find(std::make_pair(PARSER_LOCATION, std::vector<std::string>(1, "/")));
	if (it == _blocks.end())
	{
		std::cerr << "Error: " << filename << ": No 'location /' block.\n";
		return false;
	}
	else if (it->second.conf.find(PARSER_ROOT) == it->second.conf.end() &&
			_blocks[server].conf.find(PARSER_ROOT) == _blocks[server].conf.end())
	{
		std::cerr << "Error: " << filename << ": No root in the 'location /' block neither in the server block.\n";
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
