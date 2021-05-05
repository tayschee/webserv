#include "server.hpp"

// defining all functions used for canonique mode.

server::server(){}

server::server(const server& other)
{
	port = other.port;
    ip = other.ip;
    domain_name = other.domain_name; //to replace ip adress
	socket_host = other.socket_host; //to identify host
	socket_client = other.socket_client; //to identify clients
	address = other.address;
	pars = other.pars;
}

server &server::operator=(const server& other)
{
	(void)other;
	return *this;
}

// defining "normal" constructor used to create the server.
// _ip is given as a string under ipv4 format.
server::server(const parser & _pars) :
	port(ft_atoi<int>(_pars.get_block("server").conf.find("listen")->second[0])),
	domain_name("server"), socket_client(), pars(_pars)
{
	std::string _ip(pars.get_block("server").conf.find("server_name")->second[0]);
	std::string bytes[4];
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
		ip |= atoi(bytes[i].c_str()) << (8 * i);

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = ip;

	socket_host = socket(AF_INET, SOCK_STREAM, 0);
	int opt = true;
	if (setsockopt(socket_host, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) // Enable reuse of address, fixes the error "address is already in use"
		std::cerr << "failed to set socket options. Error: " << strerror(errno) << std::endl;

	if (bind(socket_host, (struct sockaddr *)&address, sizeof(address)))
		std::cerr << "failed to bind on port " << port << ". Error: " << strerror(errno) << std::endl;
}

server::~server()
{
	//for (std::list<client>::iterator it = socket_client.begin(); it != socket_client.end(); it++)
	//	it = close_client(it);
	//close(socket_host);
}
