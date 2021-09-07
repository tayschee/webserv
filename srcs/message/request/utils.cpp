#include "message/request.hpp"

void	request::request_line::clear()
{
	method.clear();
	uri.clear();
	query_string.clear();
	version.clear();
}

void	request::clear()
{
	first_line.clear();
	header.clear();
	body.clear();
}

int		request::check_path(const std::string &path) const
{
	std::vector<std::string> split_path(split(path, "/"));
	int score = 0;
	size_t i = 0;

	while (i < split_path.size())
	{
		if (split_path[i] == "..")
		{
			--score;
			if (score <= 0)
				return 1;
		}
		else 
		{
			if (score == INT_MAX)
				return 0;
			++score;
		}
		++i                                                                         ;
	}
	std::cout << "hello\n";
	return 0;
}