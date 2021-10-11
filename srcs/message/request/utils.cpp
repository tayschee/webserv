#include "message/request.hpp"

void	request::request_line::clear()
{
	method = "";
	uri = "";
	query_string = "";
	version = "";
}

void	request::clear()
{
	first_line.clear();
	header.clear();
	body = "";
}

int		request::path_is_valid(const std::string &path) const
{
	std::vector<std::string> split_path(split(path, "/"));
	size_t i = 0;
	int		count = 0;
	while (i < split_path.size())
	{
		if (split_path[i] == "..")
		{
			--count;
			if (count < 0)
				return 1;
		}
		else
		{
			if (count == __INT_MAX__)
			{
				return 1;
			}
			++count;
		}
		++i;
	}
	return 0;
}