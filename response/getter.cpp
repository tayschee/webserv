# include <message/response.hpp>

response::response_line	response::get_first_line() const
{
	return first_line;
}

int						response::get_status() const
{
	return first_line.status;
}

std::string				response::get_status_string() const
{
	return first_line.status_string;
}
std::string				response::get_version() const
{
	return first_line.version;
}

/*get response, hf == header_field, by default hf_sep == ": " and eol == end_of_line by default eol == CRLF*/
std::string				response::get(const std::string &hf_sep, const std::string &eol) const
{
	std::string resp_str;
	iterator	it(header.begin());
	iterator	end(header.end());

	resp_str = first_line.version + " " + ft_itoa(first_line.status) + " " + first_line.status_string + eol;

	while (it != end)
	{
		resp_str += it->first + hf_sep + it->second + eol;
		++it;
	}
	resp_str += eol;
	resp_str += body;

	return resp_str;
}