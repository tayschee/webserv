#include "message/request.hpp"

/*use to have all information about first line in the structure of request to store first_line*/
request::request_line	request::get_first_line() const
{
	return first_line;
}

/*more specific than get_first_line simply use for method*/
std::string		request::get_method() const
{
	return first_line.method;
}

/*more specific than get_first_line simply use for uri*/
std::string		request::get_uri() const
{
	return first_line.uri;
}

/*more specific than get_first_line simply use for version*/
std::string		request::get_version() const
{
	return first_line.version;
}

/*to have request information in the form of std::string*/
std::string		request::get() const
{
	const_iterator	it(header.begin());
	const_iterator	end(header.end());
	std::string req_str;

	req_str = first_line.method + " " + first_line.uri + " " + first_line.version + "\n";

	while (it != end)
	{
		req_str += it->first + ": " + it->second + "\n";
		++it;
	}
	req_str += "\n" + body;

	return req_str;
}

/*to have response to this request (you must send it)*/
response		request::get_response(const parser &pars) const
{
	response	resp(*this, pars);

	return resp;
}