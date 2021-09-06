#include "server.hpp"

// defining all functions used for canonique mode.

server::server(){} // constructor by default

server::server(const server& other) // constructor by copy
{
	(void)other;
}

server &server::operator=(const server& other) // assign
{
	(void)other;
	return *this;
}

// defining "normal" constructor used to create the server.
// _ip is given as a string under ipv4 format.
server::server(const parser & _pars) :
port(ft_atoi<int>(_pars.get_block(PARSER_SERVER).conf.find(PARSER_LISTEN)->second[0])), domain_name("server")
{
	std::string _ip(_pars.get_block(PARSER_SERVER).conf.find(PARSER_HOST)->second[0]);

	ip = ip_resolution(_ip);
	/*std::string bytes[4];
	int index = 0;

	for (size_t i = 0; i < _ip.size(); i++)
	{
		if (::isdigit(_ip[i]))
			bytes[index].append(1, _ip[i]); // Filling the current byte of the ip address
		else
			index++;
	}

	ip = 0;
	for (int i = 0; i < 4; i++)
		ip |= atoi(bytes[i].c_str()) << (8 * i);*/

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = ip;

	socket_host = socket(AF_INET, SOCK_STREAM, 0);
	int opt = true;
	if (setsockopt(socket_host, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) // Enable reuse of address, fixes the error "address is already in use"
		throw std::string(std::string("failed to set socket options. Error: ") + strerror(errno) + "\n");

	if (bind(socket_host, (struct sockaddr *)&address, sizeof(address)))
		throw std::string("failed to bind on port " + _ip + ":" + ft_itoa(port) + ". Error: " + strerror(errno) + "\n");
}

server::~server(){} // destructor
