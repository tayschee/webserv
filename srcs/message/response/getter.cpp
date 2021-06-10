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

/*get response, hf == header_field, by default hf_sep == ": " and eol == end_of_line by default eol == CRLF*/

std::string		response::get(const std::string &hf_sep, const std::string &eol) const
{
	std::string resp_str;
	const_iterator	it(header.begin());
	const_iterator	end(header.end());
	resp_str = first_line.version + " " + ft_itoa(first_line.status) + " " + first_line.status_string + eol;
	//std::cout << resp_str << std::endl;
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
	//std::cout << resp_str << std::endl;
	if (first_time == 1)
	{
		const_iterator	it(header.begin());
		const_iterator	end(header.end());
		resp_str = first_line.version + " " + ft_itoa(first_line.status) + " " + first_line.status_string + eol;
		while (it != end)
		{
			resp_str += it->first + hf_sep + it->second + eol;
			++it;
		}
		resp_str += eol;
	}
	if (header.find(TRANSFERT_ENCODING) == header.end())
	{
		resp_str += body;
		first_time = 0;
	}
	else
	{
		size_t pos = 0;
		std::string base = HEXADECIMAL_BASE;
		size_t		size = 100;
		size_t		end = body.size() > 100 ? (body.size() / size) * size : 0;
		std::string ssize(ft_itoa_base(size, base));
		while (pos < end)
		{
			resp_str += ssize + "\r\n" + body.substr(pos, size) + "\r\n";
			pos += size;
			first_time++;
    		send(fd, resp_str.c_str(), resp_str.size(), 0);
			resp_str = "";
		}
		if (pos < body.size())
		{
			size = body.size() - pos;
			resp_str += ft_itoa_base(size, base) + "\r\n" + body.substr(pos, size) + "\r\n";
    		send(fd, resp_str.c_str(), resp_str.size(), 0);
			resp_str = "";
		}
		resp_str += "0\r\n\r\n";
		first_time = 0;
	}
    send(fd, resp_str.c_str(), resp_str.size(), 0);
	return first_time;
}