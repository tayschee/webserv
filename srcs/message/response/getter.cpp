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
	std::string resp_str;
	const_iterator	it(header.begin());
	const_iterator	end(header.end());
	resp_str = first_line.version + " " + ft_itoa(first_line.status) + " " + first_line.status_string + eol;
	while (it != end)
	{
		resp_str += it->first + hf_sep + it->second + eol;
		++it;
	}

	return resp_str;
}

int		response::sent(int fd, const std::string &hf_sep, const std::string &eol)
{
	std::string resp_str;

	std::cout << "test\n";
	const_iterator	it(header.begin());
	const_iterator	end(header.end());
	std::cout << "test2\n";
	resp_str = first_line.version + " " + ft_itoa(first_line.status) + " " + first_line.status_string + eol;
	//std::cout << resp_str << std::endl;
	std::cout << "test3\n";
	while (it != end)
	{
		resp_str += it->first + hf_sep + it->second + eol;
		++it;
	}
	std::cout << "test4\n";
	resp_str += eol;
	resp_str += body;
	std::cout << "test5.1\n";
    write(fd, resp_str.c_str(), resp_str.size());
	std::cout << "test5.2\n";
	std::cout << "abc\n";
	return 0;
}