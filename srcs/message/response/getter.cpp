#include "message/response.hpp"

const response::response_line	&response::get_first_line() const
{
	return first_line;
}

int						response::get_status() const
{
	return first_line.status;
}

const std::string				&response::get_status_string() const
{
	return first_line.status_string;
}
const std::string				&response::get_version() const
{
	return first_line.version;
}

std::string		response::get(const std::string &hf_sep, const std::string &eol) const
{
	const std::string list[] = {SERVER, DATE, CONTENT_TYPE, CONTENT_LENGTH, LAST_MODIFIED, LOCATION, CONNECTION, "keep-alive"};
	std::vector<std::string> vec(list, &list[6]);

	std::string resp_str;
	const_iterator	it(header.begin());
	const_iterator	end(header.end());
	resp_str = first_line.version + " " + ft_itoa(first_line.status) + " " + first_line.status_string + eol;
	resp_str += header_in_order(hf_sep, eol, vec);
	size_t i;

	std::cout << "resp_str 1: " << resp_str << "\n";
	while (it != end)
	{
		std::cout << "salut\n";
		i = 0;
		while (i < vec.size())
		{
			if (it->first == vec[i])
				break;
			++i;
		}
		if (i == vec.size())
			resp_str += it->first + hf_sep + it->second + eol;
		++it;
	}
	std::cout << "resp_str 2: " << resp_str << "\n";
	resp_str += eol;
	resp_str += body;
	return resp_str;
}

int		response::sent(int fd, const std::string &hf_sep, const std::string &eol)
{
	std::string resp_str;

	std::cout << "test\n";
	std::cout << "test2\n";
	resp_str = get(hf_sep, eol);
    write(fd, resp_str.c_str(), resp_str.size());
	std::cout << "test5.2\n";
	std::cout << "abc\n";
	return 0;
}