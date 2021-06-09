#include "message/request.hpp"
#include "message/response.hpp"

/*use to have all information about first line in the structure of request to store first_line*/
const request::request_line	&request::get_first_line() const
{
	return first_line;
}

/*more specific than get_first_line simply use for method*/
const std::string		&request::get_method() const
{
	return first_line.method;
}

/*more specific than get_first_line simply use for uri*/
const std::string	&request::get_uri() const
{
	return first_line.uri;
}

/*more specific than get_first_line simply use for query*/
const std::string	request::get_query() const
{
	return first_line.query_string;
}

/*more specific than get_first_line simply use for version*/
const std::string		&request::get_version() const
{
	return first_line.version;
}

const std::string	request::get_content_type() const
{
	if (header.find(CONTENT_TYPE) == header.end())
		return "";
	return header.find(CONTENT_TYPE)->second;
}

const std::string	request::get_auth_type() const
{
	if (header.find(AUTH_BASIC) == header.end())
		return "";
	return header.find(AUTH_BASIC)->second;
}

const std::string	request::get_user() const
{
	if (header.find(AUTHORIZATION) == header.end())
		return "";
	return header.find(AUTHORIZATION)->second;
}

const std::string	request::get_content_length() const
{
	if (header.find(CONTENT_LENGTH) == header.end())
		return "";
	return header.find(CONTENT_LENGTH)->second;
}

const std::string	request::get_host() const
{
	if (header.find(HOST) == header.end())
		return "";
	return header.find(HOST)->second;
}

/*to have request information in the form of std::string*/
std::string		request::get(const std::string &hf_sep, const std::string &eol) const
{
	const_iterator	it(header.begin());
	const_iterator	end(header.end());
	std::string req_str;

	req_str = first_line.method + " " + first_line.uri + " " + first_line.version + eol;

	while (it != end)
	{
		req_str += it->first + hf_sep + it->second + eol;
		++it;
	}
	req_str += eol + body;

	return req_str;
}

/*to have response to this request (you must send it)*/
response		request::get_response(const parser &pars) const
{
	response	resp(*this, pars);

	return resp;
}