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

int		 request::validity(const parser &pars) const
{
	size_t i(0);

	if (first_line.version != HTTP_VERSION)
		return 505;
	header_type::const_iterator end(header.end());
	std::pair<header_type::const_iterator, header_type::const_iterator> range_host(header.equal_range(HOST));

	if (range_host.first == end || end != range_host.second)
		return 400;

	(void)pars;
	std::vector<std::string> server_name_vec(pars.get_block(PARSER_SERVER).conf.find(PARSER_HOST)->second);
	while (i < server_name_vec.size())
	{
		if (range_host.first->second == server_name_vec[i])
			break;
		if (!(i + 1 < server_name_vec[i].size()))
			return 400;
		++i;
	}
	
	//if (ft_atoi<size_t>(pars.get_block(PARSER_SERVER).conf.find(BUFFER_SIZE)->second[0]) < body.size())
	//	return 413;
	//maybe check tf
	return 0;
}