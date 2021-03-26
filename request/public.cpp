#include <request.hpp>


/*execute method store in method[CMD]*/
std::string	request::execute()
{
	const std::string possible_cmd[] = {"GET", "HEAD", "POST", "PUT", "DELETE",
										"CONNECT", "OPTIONS", "TRACE", "PACTH"};

	return "";
}

/*iterator pointing on first information about header except cmd*/
request::iterator request::begin() const
{
	return (header.begin());
}

/*iterator pointing on element after last information on header*/
request::iterator request::end() const
{
	return (header.end());
}