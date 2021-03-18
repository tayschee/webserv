/*  this file regroup of coplien's function (constructor, destructor, operator=)
    in server.hpp 
*/

#include "server.hpp"
#include <iostream>

#include <arpa/inet.h>
#include <netinet/ip.h>

// defining all functions used for canonique mode.
server::server()
{}

server::server(const server& other)
{
	(void)other;
}

server &server::operator=(const server& other)
{
	(void)other;
	return *this;
}

// defining the "normal" constructor used to create the server.
// _ip is given as a string under ipv4 format.
server::server(const std::string& _domain, const std::string& _ip, int _port) : domain_name(_domain), port(_port)
{
	std::string bytes[4];
	int index = 0;

	for (int i = 0; i < _ip.size(); i++)
	{
		if (::isdigit(_ip[i]))
			bytes[index].append(1, _ip[i]);
		else
			index++;
	}

	ip = 0;
	for (int i = 0; i < 4; i++)
		ip |= std::stoi(bytes[i]) << (8 * i);
}

server::~server()
{
}
