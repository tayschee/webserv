#include "message/request.hpp"

int		request::receive(const int socket, receive_management &recv_data)
{
	int i;

	try
	{
		i = recv_data.receive(socket, this);
	}
	catch(const std::exception& e)
	{
		i = 500; //la valeur peut fluctuer
		recv_data.clear();
	}

	return i;
}

int		request::validity() const
{
	if (first_line.version != HTTP_VERSION)
		return 505;
	header_type::const_iterator end(header.end());
	std::pair<header_type::const_iterator, header_type::const_iterator> range_host(header.equal_range(HOST));

	if (range_host.first == end)
		return 400;
	else if (++range_host.first != range_host.second)
		return 400;
	//maybe check tf
	return 0;
}