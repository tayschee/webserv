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

int		request::validity(const parser &pars) const
{
	if (first_line.version != HTTP_VERSION)
		return 505;
	header_type::const_iterator end(header.end());
	std::pair<header_type::const_iterator, header_type::const_iterator> range_host(header.equal_range(HOST));

	if (range_host.first == end || ++range_host.first != range_host.second)
		return 400;
	if (ft_atoi<size_t>(pars.get_block(PARSER_SERVER).conf.find("body_size")->second[0]) < body.size())
		return 413;
	//maybe check tf
	return 0;
}