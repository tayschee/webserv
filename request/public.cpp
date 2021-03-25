#include <request.hpp>

/*return cmd*/
std::string		request::get_cmd() const
{
	return (cmd);
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