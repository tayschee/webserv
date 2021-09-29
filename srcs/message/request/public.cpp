#include "message/request.hpp"

int		 request::validity() const
{
	if (first_line.version.empty())
		return 400;
	if (first_line.uri.size() < 1 || first_line.uri[0] != '/')
	{
		return 400;
	}
	if (first_line.version != HTTP_VERSION)
		return 505;
	if (path_is_valid(first_line.uri))
		return 400;

	header_type::const_iterator end(header.end());
	std::pair<header_type::const_iterator, header_type::const_iterator> range_host(header.equal_range(HOST));

	if (range_host.first == end || ++range_host.first != range_host.second)
	{
		return 400;
	}
	
	return 200;
}