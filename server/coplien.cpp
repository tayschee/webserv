#include "server.hpp"

// defining all functions used for canonique mode.

server::server(){}

server::server(const server& other)
{
	(void)other;
}

server &server::operator=(const server& other)
{
	(void)other;
	return *this;
}

// defining "normal" constructor used to create the server.
// _ip is given as a string under ipv4 format.
server::server(const std::string& _domain, const std::string& _ip, int _port) :
	port(_port), domain_name(_domain),
	socket_client(512, 0)
{
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
	for (size_t i = 0; i < socket_client.size(); i++)
		close(socket_client[i]);
	close(socket_host);
}
