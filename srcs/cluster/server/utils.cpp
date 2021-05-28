#include "server.hpp"

/*transforme ip in the form of string to int to be use by server function */
int 	server::ip_resolution(const std::string &ip) const
{
	std::string bytes[4];
	int index = 0;
	int ip_int(0);

	for (size_t i = 0; i < ip.size(); i++)
	{
		if (::isdigit(ip[i]))
			bytes[index].append(1, ip[i]); // Filling the current byte of the ip address
		else
			index++;
	}
	for (int i = 0; i < 4; i++)
		ip_int |= atoi(bytes[i].c_str()) << (8 * i);

	return ip_int;
}

int		server::get_socket_host() const // get host
{
    return socket_host;
}