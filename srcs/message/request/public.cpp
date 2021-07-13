#include "message/request.hpp"

/*int		request::receive(const int socket, receive_management &recv_data)
{
	int i;

	try
	{
		std::cout << "np1\n";
		i = recv_data.receive(socket);
		std::cout << "np2\n";
	}
	catch(const std::exception& e)
	{
		i = 500; //la valeur peut fluctuer
		recv_data.clear();
	}
	return i;
}*/

int		 request::validity() const
{
	if (first_line.version != HTTP_VERSION)
		return 505;
	header_type::const_iterator end(header.end());
	std::pair<header_type::const_iterator, header_type::const_iterator> range_host(header.equal_range(HOST));

	if (range_host.first == end || end != range_host.second)
		return 400;
	
	//if (ft_atoi<size_t>(pars.get_block(PARSER_SERVER).conf.find(BUFFER_SIZE)->second[0]) < body.size())
	//	return 413;
	//maybe check tf
	return 0;
}

/*const std::vector<parser>::const_iterator request::find_parser(const std::vector<parser> &parser_vec) const
{
	std::string host(header.find(HOST)->second);
	std::vector<parser>::const_iterator it(parser_vec.begin());
	std::vector<parser>::const_iterator end(parser_vec.end());

	while (it != end)
	{
		if (it->get_block(PARSER_SERVER).conf.find(PARSER_SERVER_NAME)->second == host)
			return it;
		++it;
	}
	return it;
}*/